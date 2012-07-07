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

#include "gui/widgets/listbox.h"

#include "configuration.h"

#include "gui/gui.h"
#include "gui/palette.h"
#include "gui/sdlinput.h"

#include "resources/theme.h"

#include "log.h"

#include <guichan/font.hpp>
#include <guichan/graphics.hpp>
#include <guichan/key.hpp>
#include <guichan/listmodel.hpp>

float ListBox::mAlpha = 1.0;

ListBox::ListBox(gcn::ListModel *listModel):
    gcn::ListBox(listModel),
    mFont(gui->getFont()),
    mSelected(-1)
{
    recalculateHeight();
}

ListBox::~ListBox()
{
}

void ListBox::updateAlpha()
{
    float alpha = std::max(config.getFloatValue("guialpha"),
                           Theme::instance()->getMinimumOpacity());

    if (mAlpha != alpha)
        mAlpha = alpha;
}

void ListBox::draw(gcn::Graphics *graphics)
{
    if (!mListModel)
        return;

    updateAlpha();

    graphics->setFont(getFont());

    int height, y;

    // Draw the list elements
    for (int i = 0; i < mListModel->getNumberOfElements(); ++i)
    {
        height = getRowHeight(i);
        drawRow(graphics, y, i, height);
        y += height;
    }
}

void ListBox::drawRow(gcn::Graphics *graphics, int y, int i, int rowHeight)
{
    if (i == mSelected)
    {
        // set color for selection
        graphics->setColor(Theme::getThemeColor(Theme::HIGHLIGHT,
                                                (int) (mAlpha * 255.0f)));
        graphics->fillRectangle(gcn::Rectangle(0, y, getWidth(),
                                getRowHeight(i)));
        // set back to text color
        graphics->setColor(Theme::getThemeColor(Theme::TEXT));
    }
    graphics->drawText(mListModel->getElementAt(i), 1, y);
}

void ListBox::setSelected(int selected)
{
    if (!mListModel || selected < 0)
    {
        mSelected = -1;
    }
    else if (selected >= mListModel->getNumberOfElements())
    {
        mSelected = mListModel->getNumberOfElements() - 1;
    }
    else
    {
        mSelected = selected;
    }

    gcn::Rectangle scroll;

    int height = 0;
    int y = 0;
    for (int i = 0; i < mSelected; ++i)
    {
        height = getRowHeight(i);
        y += height;
    }
    scroll.y = y;

    scroll.height = height;
    showPart(scroll);

    distributeValueChangedEvent();
}

void ListBox::keyPressed(gcn::KeyEvent& keyEvent)
{
    gcn::Key key = keyEvent.getKey();

    if (key.getValue() == Key::ENTER || key.getValue() == Key::SPACE)
    {
        distributeActionEvent();
        keyEvent.consume();
    }
    else if (key.getValue() == Key::UP)
    {
        if (getSelected() > 0)
            setSelected(mSelected - 1);
        else if (getSelected() == 0 && mWrappingEnabled)
            setSelected(getListModel()->getNumberOfElements() - 1);
        keyEvent.consume();
    }
    else if (key.getValue() == Key::DOWN)
    {
        if (getSelected() < (getListModel()->getNumberOfElements() - 1))
            setSelected(mSelected + 1);
        else if (getSelected() == (getListModel()->getNumberOfElements() - 1) &&
                 mWrappingEnabled)
            setSelected(0);
        keyEvent.consume();
    }
    else if (key.getValue() == Key::HOME)
    {
        setSelected(0);
        keyEvent.consume();
    }
    else if (key.getValue() == Key::END)
    {
        setSelected(getListModel()->getNumberOfElements() - 1);
        keyEvent.consume();
    }
}

void ListBox::mousePressed(gcn::MouseEvent &mouseEvent)
{
    if (mouseEvent.getButton() != gcn::MouseEvent::LEFT)
        return;

    int mouseY = std::max(0, mouseEvent.getY());

    int id = getIdByY(mouseY);
    setSelected(id);
    distributeActionEvent();
}

void ListBox::mouseWheelMovedUp(gcn::MouseEvent &mouseEvent)
{
    // Don't do anything on scrollwheel. ScrollArea will deal with that.
}

void ListBox::mouseWheelMovedDown(gcn::MouseEvent &mouseEvent)
{
    // Don't do anything on scrollwheel. ScrollArea will deal with that.
}

void ListBox::mouseDragged(gcn::MouseEvent &event)
{
    if (event.getButton() != gcn::MouseEvent::LEFT)
        return;

    // Make list selection update on drag, but guard against negative y
    int y = std::max(0, event.getY());
    setSelected(getIdByY(y));
}

int ListBox::getIdByY(int y)
{
    int height;
    int yCoord = 0;
    for (int i = 0; i < mListModel->getNumberOfElements(); ++i)
    {
        height = getRowHeight(i);
        if (y < yCoord + height)
        {
            return i;
        }
        yCoord += height;
    }
    return -1;
}

void ListBox::logic()
{
    recalculateHeight();
}

void ListBox::recalculateHeight()
{
    if (!mListModel)
        return;
    int y = 0;
    for (int i = 0; i < mListModel->getNumberOfElements();
         ++i)
    {
        y += getRowHeight(i);
    }
    setHeight(y);
}
