/*
 *  The Mana Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2012  The Mana Developers
 *
 *  This file is part of The Mana Client.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef LISTBOX_H
#define LISTBOX_H

#include <guichan/font.hpp>

#include <guichan/widgets/listbox.hpp>

class SelectionListener;

/**
 * A list box, meant to be used inside a scroll area. Same as the Guichan list
 * box except this one doesn't have a background, instead completely relying
 * on the scroll area. It also adds selection listener functionality.
 *
 * \ingroup GUI
 */
class ListBox : public gcn::ListBox
{
    public:
        ListBox(gcn::ListModel *listModel);

        ~ListBox();

        /**
         * Sets the font to render the text in.
         *
         * @param font the font to use.
         */
        void setFont(gcn::Font *font)
        {
            mFont = font;
        }

        /**
         * Draws the list box.
         */
        void draw(gcn::Graphics *graphics);

        /**
         * Update the alpha value to the graphic components.
         */
        void updateAlpha();

        /**
         * Returns height of row i
         * @param i index of row
         */
        virtual unsigned getRowHeight(int i) const
        { return mFont->getHeight(); }

        /**
         * Draws the row i
         * @param y The current y position (draw your row below this)
         * @param i The index of rendered row
         * @param rowHeight The height that the row has available
         */
        virtual void drawRow(gcn::Graphics *graphics, int y, int i,
                             int rowHeight);

        void setSelected(int selected);

        int getSelected()
        {return mSelected; }

        // Inherited from KeyListener

        void keyPressed(gcn::KeyEvent& keyEvent);

        // Inherited from MouseListener

        void mousePressed(gcn::MouseEvent& mouseEvent);

        void mouseWheelMovedUp(gcn::MouseEvent& mouseEvent);

        void mouseWheelMovedDown(gcn::MouseEvent& mouseEvent);

        void mouseDragged(gcn::MouseEvent &event);

        virtual void logic();

    private:
        gcn::Font *mFont;

        void recalculateHeight();

    protected:
        int getIdByY(int y);

        static float mAlpha;
        int mSelected;
};

#endif
