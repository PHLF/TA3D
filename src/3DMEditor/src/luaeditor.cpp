#include "luaeditor.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QFileDialog>
#include <QMessageBox>
#include <QTabWidget>
#include <QSyntaxHighlighter>
#include <QLineEdit>
#include "scripts/unit.script.h"
#include "logs.h"

using namespace TA3D;

class LuaSyntaxHighlighter : public QSyntaxHighlighter
{
public:
    LuaSyntaxHighlighter(QTextEdit *parent) : QSyntaxHighlighter(parent)   {}
protected:
    virtual void highlightBlock ( const QString & text );
    void colorize(const QString &text, QTextCharFormat &tokenFormat, QRegExp expression);
    void colorizeSingleLine(const QString &text, QTextCharFormat &tokenFormat, QRegExp startExpression, QRegExp endExpression);
};

LuaEditor *LuaEditor::pInstance = NULL;

LuaEditor *LuaEditor::instance()
{
    if (!pInstance)
        pInstance = new LuaEditor;
    return pInstance;
}

LuaEditor::LuaEditor()
{
    updating = true;
    code = new QTextEdit;
    output = new QTextEdit;
    output->setReadOnly(true);

    code->setWordWrapMode(QTextOption::NoWrap);
    code->setTabStopWidth(20);
    output->setWordWrapMode(QTextOption::NoWrap);
    QPalette pal;
    pal.setColor(QPalette::Active, QPalette::Base, Qt::black);
    pal.setColor(QPalette::Active, QPalette::Text, Qt::gray);
    output->setPalette(pal);
    new LuaSyntaxHighlighter(code);

    setLayout( new QVBoxLayout );
    QTabWidget *tabs = new QTabWidget;
    layout()->addWidget(tabs);

    QWidget *codeTab = new QWidget;
    QVBoxLayout *curLayout = new QVBoxLayout;
    codeTab->setLayout( curLayout );
    curLayout->addWidget(code);
    QHBoxLayout *codeLayout = new QHBoxLayout;
    QPushButton *bSave = new QPushButton(QIcon("icons/save.png"), tr("&save program"));
    QPushButton *bLoad = new QPushButton(QIcon("icons/open.png"), tr("&load program"));
    QPushButton *bNew = new QPushButton(QIcon("icons/new.png"), tr("&load template"));
    codeLayout->addWidget(bSave);
    codeLayout->addWidget(bLoad);
    codeLayout->addWidget(bNew);
    curLayout->addLayout(codeLayout);
    tabs->addTab( codeTab, tr("&Code"));

    QWidget *outputTab = new QWidget;
    curLayout = new QVBoxLayout;
    outputTab->setLayout( curLayout );
    curLayout->addWidget(output);

    commandInput = new QLineEdit;
    curLayout->addWidget(commandInput);

    QWidget *bottom = new QWidget;
    curLayout->addWidget(bottom);
    QHBoxLayout *bottomLayout = new QHBoxLayout(bottom);

    QPushButton *bOutput = new QPushButton(tr("&Build"));
    bOutput->setMaximumWidth(100);
    bottomLayout->addWidget(bOutput);
    bRun = new QPushButton(tr("&Start"));
    bRun->setMaximumWidth(100);
    bRun->setCheckable(true);
    bottomLayout->addWidget(bRun);
    QPushButton *bStep = new QPushButton(tr("&Step"));
    bStep->setMaximumWidth(100);
    bottomLayout->addWidget(bStep);
    tabs->addTab( outputTab, tr("&Console"));

    updateWindowTitle();
    resize(600, height());

    connect(commandInput, SIGNAL(returnPressed()), this, SLOT(runLuaCommand()));
    connect(bOutput, SIGNAL(clicked()), this, SLOT(compileCode()));
    connect(bRun, SIGNAL(toggled(bool)), this, SLOT(toggleTimer(bool)));

    connect(bSave, SIGNAL(clicked()), this, SLOT(saveProgram()));
    connect(bLoad, SIGNAL(clicked()), this, SLOT(loadProgram()));
    connect(bNew, SIGNAL(clicked()), this, SLOT(loadTemplate()));

    luaTimer.setInterval(1000 / 30);            // Run at game speed 30 ticks/sec
    luaTimer.setSingleShot(false);
    connect(&luaTimer, SIGNAL(timeout()), this, SLOT(runLuaCode()));

    updating = false;

    stream.setString(&logs, QIODevice::WriteOnly);
}

void LuaEditor::toggleTimer(bool state)
{
    if (state)
    {
        luaTimer.start();
        bRun->setText(tr("&Stop"));
    }
    else
    {
        luaTimer.stop();
        bRun->setText(tr("&Start"));
    }
}

void LuaEditor::updateWindowTitle()
{
    setWindowTitle(tr("Lua editor"));
}

void LuaEditor::updateGUI()
{
    updating = true;
    output->setText(logs);
    updating = false;
    updateWindowTitle();
}

void LuaEditor::compileCode()
{
    logs.clear();
    LOG_INFO(tr("building Lua code"));
    UnitScript::instance()->load(code->toPlainText());
    updateGUI();
}

void LuaEditor::saveProgram()
{
    QString filename = QFileDialog::getSaveFileName(this, tr("save Lua progam as"), QString(), tr("all files(*.*);;Lua programs(*.lua)"));
    if (filename.isEmpty())
        return;
    QFile file(filename);
    file.open(QIODevice::WriteOnly);
    if (!file.exists() || !file.isOpen())
    {
        QMessageBox::critical(this, tr("Error opening file"), tr("Error: could not open file %1 for writing").arg(filename));
        return;
    }
    file.write(code->toPlainText().toAscii());
    file.close();
}

void LuaEditor::loadProgram()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("load Lua progam"), QString(), tr("all files(*.*);;Lua programs(*.lua)"));
    if (filename.isEmpty())
        return;
    QFile file(filename);
    file.open(QIODevice::ReadOnly);
    if (!file.exists() || !file.isOpen())
    {
        QMessageBox::critical(this, tr("Error opening file"), tr("Error: could not read file %1").arg(filename));
        return;
    }
    code->setPlainText(QString(file.readAll()));
    file.close();
}

void LuaSyntaxHighlighter::highlightBlock(const QString &text)
{
    if (text.isEmpty())
        return;
    // type qualifiers
    {
        QTextCharFormat tokenFormat;
        tokenFormat.setForeground(QColor::fromRgb(0x7F,0x0,0x0));

        QRegExp token("\\blocal\\b");
        colorize(text, tokenFormat, token);
    }
    // other keywords
    {
        QTextCharFormat tokenFormat;
        QPen pen;
        pen.setWidthF(1.0);
        pen.setColor(QColor::fromRgb(0x7F,0x7F,0x0));
        tokenFormat.setTextOutline(pen);

        QRegExp token("\\bdo\\b"
                      "|\\bend\\b"
                      "|\\brepeat\\b"
                      "|\\buntil\\b"
                      "|\\bif\\b"
                      "|\\bthen\\b"
                      "|\\belse\\b"
                      "|\\belseif\\b"
                      "|\\bfor\\b"
                      "|\\bfunction\\b"
                      "|\\breturn\\b"
                      "|\\bbreak\\b"
                      "|\\band\\b"
                      "|\\bor\\b"
                      "|\\bnot\\b"
                      "|\\bin\\b");
        colorize(text, tokenFormat, token);
    }
    // preprocessor
    {
        QTextCharFormat tokenFormat;
        tokenFormat.setForeground(QColor::fromRgb(0,0,0x7F));

        QRegExp token("\\b#include\\b");
        colorize(text, tokenFormat, token);
    }
    // Values
    {
        QTextCharFormat tokenFormat;
        QPen pen;
        pen.setWidthF(1.0);
        pen.setColor(QColor::fromRgb(0x0,0x0,0x7F));
        tokenFormat.setTextOutline(pen);

        QRegExp token("\\btrue\\b"
                      "|\\bfalse\\b"
                      "|\\bnil\\b"
                      "|\\b\\d+\\b"
                      "|\\b0x\\d+\\b"
                      "|\\b0X\\d+\\b");
        colorize(text, tokenFormat, token);
    }
    // Comments
    {
        QTextCharFormat tokenFormat;
        QPen pen;
        pen.setWidthF(1.0);
        pen.setColor(QColor::fromRgb(0x7F,0x7F,0x7F));
        tokenFormat.setTextOutline(pen);

        colorizeSingleLine(text, tokenFormat, QRegExp("--"), QRegExp("\\n"));
    }
}

void LuaSyntaxHighlighter::colorize(const QString &text, QTextCharFormat &tokenFormat, QRegExp expression)
{
    int startIndex = 0;
    startIndex = text.indexOf(expression);

    while (startIndex >= 0)
    {
        int tokenLength = expression.matchedLength();
        setFormat(startIndex, tokenLength, tokenFormat);
        startIndex = text.indexOf(expression,
                                  startIndex + tokenLength);
    }
}

void LuaSyntaxHighlighter::colorizeSingleLine(const QString &text, QTextCharFormat &tokenFormat, QRegExp startExpression, QRegExp endExpression)
{
    int startIndex = 0;
    startIndex = text.indexOf(startExpression);

    while (startIndex >= 0)
    {
        int endIndex = text.indexOf(endExpression, startIndex);
        int commentLength;
        if (endIndex == -1)
        {
            commentLength = text.length() - startIndex;
        }
        else
        {
            commentLength = endIndex - startIndex
                            + endExpression.matchedLength();
        }
        setFormat(startIndex, commentLength, tokenFormat);
        startIndex = text.indexOf(startExpression,
                                  startIndex + commentLength);
    }
}

uint32 __lua_timer = 0;

void LuaEditor::runLuaCode()
{
    uint32 msec_timer = QTime().msecsTo(QTime::currentTime());
    UnitScript::instance()->run((msec_timer - __lua_timer) * 0.001f);
    __lua_timer = msec_timer;
}

void LuaEditor::runLuaCommand()
{
    QString command = commandInput->text();
    commandInput->clear();
    getStream() << command << "\n";
    updateGUI();
    UnitScript::runCommand(command);
}

void LuaEditor::loadTemplate()
{
    if (QMessageBox::question(this, tr("Are you sure?"), tr("Load the template? If not saved, current code will be lost."), QMessageBox::Yes, QMessageBox::No) == QMessageBox::No)
        return;
    QString filename("scripts/template.lua");
    QFile file(filename);
    file.open(QIODevice::ReadOnly);
    if (!file.exists() || !file.isOpen())
    {
        QMessageBox::critical(this, tr("Error opening file"), tr("Error: could not read file %1").arg(filename));
        return;
    }
    code->setPlainText(QString(file.readAll()));
    file.close();
}