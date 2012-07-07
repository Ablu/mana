/*
 *  The Mana Client
 *  Copyright (C) 2010-2012  The Mana Developers
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

#include "gui/widgets/avatarlistbox.h"

#include "graphics.h"

#include "gui/gui.h"
#include "gui/palette.h"

#include "resources/image.h"
#include "resources/resourcemanager.h"
#include "resources/theme.h"

#include "utils/stringutils.h"

#include <guichan/font.hpp>
#include <guichan/graphics.hpp>

int AvatarListBox::instances = 0;
Image *AvatarListBox::onlineIcon = 0;
Image *AvatarListBox::offlineIcon = 0;

AvatarListBox::AvatarListBox(AvatarListModel *model):
    ListBox(model)
{
    instances++;

    if (instances == 1)
    {
        onlineIcon = Theme::getImageFromTheme("circle-green.png");
        offlineIcon = Theme::getImageFromTheme("circle-gray.png");
    }

    setWidth(200);
}

AvatarListBox::~AvatarListBox()
{
    instances--;

    if (instances == 0)
    {
        onlineIcon->decRef();
        offlineIcon->decRef();
    }
}

void AvatarListBox::drawRow(gcn::Graphics *gcnGraphics, int y, int i,
                            int rowHeight)
{
    Graphics *graphics = static_cast<Graphics*>(gcnGraphics);
    // Draw filled rectangle around the selected list element
    if (mSelected == i)
    {
        graphics->setColor(Theme::getThemeColor(Theme::HIGHLIGHT,
                                                (int) (mAlpha * 255.0f)));
        graphics->fillRectangle(gcn::Rectangle(0, y, getWidth(), rowHeight));
    }
    else
    {
        graphics->setColor(Theme::getThemeColor(Theme::TEXT));
    }

    AvatarListModel* model = static_cast<AvatarListModel*>(mListModel);

    Avatar *a = model->getAvatarAt(i);
    // Draw online status
    Image *icon = a->getOnline() ? onlineIcon : offlineIcon;
    if (icon)
        graphics->drawImage(icon, 2, y + 1);

    if (a->getDisplayBold())
        graphics->setFont(boldFont);

    std::string text;

    if (a->getMaxHp() > 0)
    {
        text = strprintf("%s %d/%d", a->getName().c_str(),
                         a->getHp(), a->getMaxHp());
    }
    else
    {
        text = a->getName();
    }

    // Draw Name
    graphics->drawText(text, 15, y);

    // TODO: resize according to max width

    if (a->getDisplayBold())
        graphics->setFont(getFont());
}

void AvatarListBox::mousePressed(gcn::MouseEvent &event)
{
    if (event.getButton() == gcn::MouseEvent::LEFT)
    {
        int y = event.getY();
        setSelected(y / getFont()->getHeight());
        distributeActionEvent();
    }
    // TODO: Add support for context menu
    else if (event.getButton() == gcn::MouseEvent::RIGHT)
    {
        // Show context menu
    }
}
