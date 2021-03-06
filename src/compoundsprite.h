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

#ifndef COMPOUNDSPRITE_H
#define COMPOUNDSPRITE_H

#include "sprite.h"

#include <vector>

class Image;

class CompoundSprite : public Sprite
{
public:
    CompoundSprite();

    ~CompoundSprite();

    virtual bool reset();

    virtual bool play(std::string action);

    virtual bool update(int time);

    virtual bool draw(Graphics *graphics, int posX, int posY) const;

    /**
     * Gets the width in pixels of the first sprite in the list.
     */
    virtual int getWidth() const
    { return mWidth; }

    /**
     * Gets the height in pixels of the first sprite in the list.
     */
    virtual int getHeight() const
    { return mHeight; }

    int getOffsetX() const
    { return mOffsetX; }

    int getOffsetY() const
    { return mOffsetY; }

    virtual const Image *getImage() const;

    virtual bool setDirection(SpriteDirection direction);

    int getNumberOfLayers() const;

    virtual bool drawnWhenBehind() const;

    int getDuration() const;

    size_t size() const
    { return mSprites.size(); }

    void addSprite(Sprite *sprite);

    void setSprite(int layer, Sprite *sprite);

    Sprite *getSprite(int layer) const
    { return mSprites.at(layer); }

    void removeSprite(int layer);

    void clear();

    void ensureSize(size_t layerCount);

    void doRedraw()
    { mNeedsRedraw = true; }

private:
    typedef std::vector<Sprite*>::iterator SpriteIterator;
    typedef std::vector<Sprite*>::const_iterator SpriteConstIterator;

    void redraw() const;

    mutable Image *mImage;
    mutable Image *mAlphaImage;

    mutable int mWidth, mHeight;
    mutable int mOffsetX, mOffsetY;

    mutable bool mNeedsRedraw;

    std::vector<Sprite*> mSprites;
};

#endif // COMPOUNDSPRITE_H
