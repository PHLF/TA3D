/*  TA3D, a remake of Total Annihilation
    Copyright (C) 2005  Roland BROCHARD

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA*/

#include "files.h"
#include <logs/logs.h>
#include <QFile>
#include <QFileInfo>
#include <QBuffer>
#include "paths.h"

namespace TA3D
{
namespace Paths
{
namespace Files
{

    bool Load(QStringList& out, const QString& filename, const uint32 sizeLimit, const bool emptyListBefore)
	{
        if (emptyListBefore)
            out.clear();
        QFile file(filename);
        file.open(QIODevice::ReadOnly);
        if (!file.isOpen())
        {
            LOG_WARNING("Impossible to open the file `" << filename << "`");
            return false;
        }
        if (sizeLimit)
        {
            if (file.size() > sizeLimit)
            {
                LOG_WARNING("Impossible to read the file `" << filename << "` (size > " << sizeLimit << ")");
                return false;
            }
        }
        while (file.bytesAvailable())
            out.push_back(QString::fromUtf8(file.readLine()));
        return true;
    }

    QIODevice* LoadContentInMemory(const QString& filename, const uint64 hardlimit)
	{
        qint64 size = QFileInfo(filename).size();
        if (0 == size)
            return NULL;
        if (size > hardlimit)
        {
            LOG_ERROR("Impossible to load the file `" << filename << "` in memory. Its size exceeds << "
                      << hardlimit / 1024 << "Ko");
            return NULL;
        }
        QFile file(filename);
        file.open(QIODevice::ReadOnly);
        QBuffer *memory_file = new QBuffer;
        memory_file->setData(file.readAll());
        memory_file->open(QIODevice::ReadOnly);
        return memory_file;
	}


    bool SaveToFile(const QString& filename, const QByteArray& content)
	{
        QFile dst(filename);
        dst.open(QIODevice::WriteOnly);
        dst.write(content);
        return true;
	}


	QString ReplaceExtension(const QString& filename, const QString& newExt)
	{
        if (filename.isEmpty())
			return QString();
        QString::size_type p = filename.lastIndexOf('.');
        if (p == -1)
            return filename + newExt;
        QString::size_type s = filename.lastIndexOf("\\/");
        if (s != -1 && p < s)
            return filename + newExt;
        return Substr(filename, 0, p) + newExt;
	}



	bool Copy(const QString& from, const QString& to, const bool overwrite)
	{
        QFile dst(to);
        if (dst.exists() && !overwrite)
            return true;
        QFile src(from);
        if (!src.exists())
            return true;
        src.open(QIODevice::ReadOnly);
        dst.open(QIODevice::WriteOnly);
        while(src.bytesAvailable())
            dst.write(src.read(65536));
        return false;
	}



} // namespace Files
} // namespace Paths
} // namespace TA3D

