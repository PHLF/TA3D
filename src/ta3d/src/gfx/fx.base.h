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
#ifndef __TA3D_GFX_FX_BASE_H__
# define __TA3D_GFX_FX_BASE_H__

# include <stdafx.h>
# include <misc/vector.h>
# include <gaf.h>
# include <ta3dbase.h>
# include <misc/camera.h>



namespace TA3D
{


    class FX
    {
    public:
        FX();
        ~FX();

        void init();

        void destroy();

        /*!
        ** \brief
        **
        ** \param anim
        ** \param p
        ** \param s
        */
        void load(const int anim, const Vector3D& p, const float s);

        /*!
        ** \brief
        **
        ** \param dt
        ** \param anims
        ** \return
        */
		bool move(const float dt, const std::vector<Gaf::Animation*> &anims);

        /*!
        ** \brief
        **
        ** \param cam
        ** \param map
        ** \param anims
        */
		void draw(Camera& cam, const std::vector<Gaf::Animation*> &anims);

		/*!
		** \brief
		**
		*/
		void drawWaterDistortions();

    public:
        //! Effect duration
        float time;
        //! Get if the effect has been played
        bool playing;
        //! Position
        Vector3D Pos;
        float size;		// Taille (proportion de l'image d'origine)
		float angle;	// Angle
        int anm;		// Animation

    private:
        /*!
        ** \brief
        */
		bool doCanDrawAnim() const;

        /*!
        ** \brief
        */
        void doDrawAnimFlash();

        /*!
        ** \brief
        */
        void doDrawAnimWave(const int animIndx);

        /*!
        ** \brief
        */
        void doDrawAnimRipple();

        /*!
        ** \brief
        */
		void doDrawAnimDefault(Camera& cam, const std::vector<Gaf::Animation*> &anims);

    }; // class FX


} // namespace TA3D


#endif // __TA3D_GFX_FX_BASE_H__
