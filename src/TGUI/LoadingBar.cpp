/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// TGUI - Texus's Graphical User Interface
// Copyright (C) 2012-2013 Bruno Van de Velde (vdv_b@tgui.eu)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#include <TGUI/Widgets.hpp>
#include <TGUI/ClickableWidget.hpp>
#include <TGUI/LoadingBar.hpp>

#include <cmath>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace tgui
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    LoadingBar::LoadingBar() :
    m_Minimum       (  0),
    m_Maximum       (100),
    m_Value         (  0),
    m_SplitImage    (false),
    m_TextSize      (0)
    {
        m_Callback.widgetType = Type_LoadingBar;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    LoadingBar::LoadingBar(const LoadingBar& copy) :
    ClickableWidget   (copy),
    m_LoadedConfigFile(copy.m_LoadedConfigFile),
    m_Minimum         (copy.m_Minimum),
    m_Maximum         (copy.m_Maximum),
    m_Value           (copy.m_Value),
    m_SplitImage      (copy.m_SplitImage),
    m_Text            (copy.m_Text),
    m_TextSize        (copy.m_TextSize)
    {
        // Copy the textures
        TGUI_TextureManager.copyTexture(copy.m_TextureBack_L, m_TextureBack_L);
        TGUI_TextureManager.copyTexture(copy.m_TextureBack_M, m_TextureBack_M);
        TGUI_TextureManager.copyTexture(copy.m_TextureBack_R, m_TextureBack_R);
        TGUI_TextureManager.copyTexture(copy.m_TextureFront_L, m_TextureFront_L);
        TGUI_TextureManager.copyTexture(copy.m_TextureFront_M, m_TextureFront_M);
        TGUI_TextureManager.copyTexture(copy.m_TextureFront_R, m_TextureFront_R);

        recalculateSize();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    LoadingBar::~LoadingBar()
    {
        if (m_TextureBack_L.data != NULL)  TGUI_TextureManager.removeTexture(m_TextureBack_L);
        if (m_TextureBack_M.data != NULL)  TGUI_TextureManager.removeTexture(m_TextureBack_M);
        if (m_TextureBack_R.data != NULL)  TGUI_TextureManager.removeTexture(m_TextureBack_R);

        if (m_TextureFront_L.data != NULL) TGUI_TextureManager.removeTexture(m_TextureFront_L);
        if (m_TextureFront_M.data != NULL) TGUI_TextureManager.removeTexture(m_TextureFront_M);
        if (m_TextureFront_R.data != NULL) TGUI_TextureManager.removeTexture(m_TextureFront_R);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    LoadingBar& LoadingBar::operator= (const LoadingBar& right)
    {
        // Make sure it is not the same widget
        if (this != &right)
        {
            LoadingBar temp(right);
            this->ClickableWidget::operator=(right);

            std::swap(m_LoadedConfigFile, temp.m_LoadedConfigFile);
            std::swap(m_Minimum,          temp.m_Minimum);
            std::swap(m_Maximum,          temp.m_Maximum);
            std::swap(m_Value,            temp.m_Value);
            std::swap(m_SplitImage,       temp.m_SplitImage);
            std::swap(m_TextureBack_L,    temp.m_TextureBack_L);
            std::swap(m_TextureBack_M,    temp.m_TextureBack_M);
            std::swap(m_TextureBack_R,    temp.m_TextureBack_R);
            std::swap(m_TextureFront_L,   temp.m_TextureFront_L);
            std::swap(m_TextureFront_M,   temp.m_TextureFront_M);
            std::swap(m_TextureFront_R,   temp.m_TextureFront_R);
            std::swap(m_Text,             temp.m_Text);
            std::swap(m_TextSize,         temp.m_TextSize);
        }

        return *this;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    LoadingBar* LoadingBar::clone()
    {
        return new LoadingBar(*this);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    bool LoadingBar::load(const std::string& configFileFilename)
    {
        m_LoadedConfigFile = configFileFilename;

        // When everything is loaded successfully, this will become true.
        m_Loaded = false;

        // Remove all textures if they were loaded before
        if (m_TextureBack_L.data != NULL)  TGUI_TextureManager.removeTexture(m_TextureBack_L);
        if (m_TextureBack_M.data != NULL)  TGUI_TextureManager.removeTexture(m_TextureBack_M);
        if (m_TextureBack_R.data != NULL)  TGUI_TextureManager.removeTexture(m_TextureBack_R);
        if (m_TextureFront_L.data != NULL) TGUI_TextureManager.removeTexture(m_TextureFront_L);
        if (m_TextureFront_M.data != NULL) TGUI_TextureManager.removeTexture(m_TextureFront_M);
        if (m_TextureFront_R.data != NULL) TGUI_TextureManager.removeTexture(m_TextureFront_R);

        // Open the config file
        ConfigFile configFile;
        if (!configFile.open(configFileFilename))
        {
            TGUI_OUTPUT("TGUI error: Failed to open " + configFileFilename + ".");
            return false;
        }

        // Read the properties and their values (as strings)
        std::vector<std::string> properties;
        std::vector<std::string> values;
        if (!configFile.read("LoadingBar", properties, values))
        {
            TGUI_OUTPUT("TGUI error: Failed to parse " + configFileFilename + ".");
            return false;
        }

        // Close the config file
        configFile.close();

        // Find the folder that contains the config file
        std::string configFileFolder = "";
        std::string::size_type slashPos = configFileFilename.find_last_of("/\\");
        if (slashPos != std::string::npos)
            configFileFolder = configFileFilename.substr(0, slashPos+1);

        // Handle the read properties
        for (unsigned int i = 0; i < properties.size(); ++i)
        {
            std::string property = properties[i];
            std::string value = values[i];

            if (property == "backimage")
            {
                if (!configFile.readTexture(value, configFileFolder, m_TextureBack_M))
                {
                    TGUI_OUTPUT("TGUI error: Failed to parse value for BackImage in section LoadingBar in " + configFileFilename + ".");
                    return false;
                }

                m_SplitImage = false;
            }
            else if (property == "frontimage")
            {
                if (!configFile.readTexture(value, configFileFolder, m_TextureFront_M))
                {
                    TGUI_OUTPUT("TGUI error: Failed to parse value for FrontImage in section LoadingBar in " + configFileFilename + ".");
                    return false;
                }
            }
            else if (property == "backimage_l")
            {
                if (!configFile.readTexture(value, configFileFolder, m_TextureBack_L))
                {
                    TGUI_OUTPUT("TGUI error: Failed to parse value for BackImage_L in section LoadingBar in " + configFileFilename + ".");
                    return false;
                }
            }
            else if (property == "backimage_m")
            {
                if (!configFile.readTexture(value, configFileFolder, m_TextureBack_M))
                {
                    TGUI_OUTPUT("TGUI error: Failed to parse value for BackImage_M in section LoadingBar in " + configFileFilename + ".");
                    return false;
                }

                m_SplitImage = true;
            }
            else if (property == "backimage_r")
            {
                if (!configFile.readTexture(value, configFileFolder, m_TextureBack_R))
                {
                    TGUI_OUTPUT("TGUI error: Failed to parse value for BackImage_R in section LoadingBar in " + configFileFilename + ".");
                    return false;
                }
            }
            else if (property == "frontimage_l")
            {
                if (!configFile.readTexture(value, configFileFolder, m_TextureFront_L))
                {
                    TGUI_OUTPUT("TGUI error: Failed to parse value for FrontImage_L in section LoadingBar in " + configFileFilename + ".");
                    return false;
                }
            }
            else if (property == "frontimage_m")
            {
                if (!configFile.readTexture(value, configFileFolder, m_TextureFront_M))
                {
                    TGUI_OUTPUT("TGUI error: Failed to parse value for FrontImage_M in section LoadingBar in " + configFileFilename + ".");
                    return false;
                }
            }
            else if (property == "frontimage_r")
            {
                if (!configFile.readTexture(value, configFileFolder, m_TextureFront_R))
                {
                    TGUI_OUTPUT("TGUI error: Failed to parse value for FrontImage_R in section LoadingBar in " + configFileFilename + ".");
                    return false;
                }
            }
            else
                TGUI_OUTPUT("TGUI warning: Unrecognized property '" + property + "' in section LoadingBar in " + configFileFilename + ".");
        }

        // Check if the image is split
        if (m_SplitImage)
        {
            // Make sure the required textures were loaded
            if ((m_TextureBack_L.data != NULL) && (m_TextureBack_M.data != NULL) && (m_TextureBack_R.data != NULL)
             && (m_TextureFront_L.data != NULL) && (m_TextureFront_M.data != NULL) && (m_TextureFront_R.data != NULL))
            {
                m_Size.x = static_cast<float>(m_TextureBack_L.getSize().x + m_TextureBack_M.getSize().x + m_TextureBack_R.getSize().x);
                m_Size.y = static_cast<float>(m_TextureBack_M.getSize().y);
            }
            else
            {
                TGUI_OUTPUT("TGUI error: Not all needed images were loaded for the loading bar. Is the LoadingBar section in " + configFileFilename + " complete?");
                return false;
            }
        }
        else // The image isn't split
        {
            // Make sure the required textures were loaded
            if ((m_TextureBack_M.data != NULL) && (m_TextureFront_M.data != NULL))
            {
                m_Size = Vector2f(m_TextureBack_M.getSize());
            }
            else
            {
                TGUI_OUTPUT("TGUI error: Not all needed images were loaded for the loading bar. Is the LoadingBar section in " + configFileFilename + " complete?");
                return false;
            }
        }

        // Calculate the size of the front image (the size of the part that will be drawn)
        recalculateSize();

        // Loading has succeeded
        return m_Loaded = true;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    const std::string& LoadingBar::getLoadedConfigFile()
    {
        return m_LoadedConfigFile;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void LoadingBar::setSize(float width, float height)
    {
        // Don't do anything when the loading bar wasn't loaded correctly
        if (m_Loaded == false)
            return;

        // A negative size is not allowed for this widget
        if (width  < 0) width  = -width;
        if (height < 0) height = -height;

        // Set the size of the loading bar
        m_Size.x = width;
        m_Size.y = height;

        // Recalculate the size of the front image
        recalculateSize();

        // Recalculate the text size
        setText(m_Text.getString());
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void LoadingBar::setMinimum(unsigned int minimum)
    {
        // Set the new minimum
        m_Minimum = minimum;

        // The minimum can never be greater than the maximum
        if (m_Minimum > m_Maximum)
            m_Maximum = m_Minimum;

        // When the value is below the minimum then adjust it
        if (m_Value < m_Minimum)
            m_Value = m_Minimum;

        // Recalculate the size of the front image (the size of the part that will be drawn)
        recalculateSize();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void LoadingBar::setMaximum(unsigned int maximum)
    {
        // Set the new maximum
        m_Maximum = maximum;

        // The maximum can never be below the minimum
        if (m_Maximum < m_Minimum)
            m_Minimum = m_Maximum;

        // When the value is above the maximum then adjust it
        if (m_Value > m_Maximum)
            m_Value = m_Maximum;

        // Recalculate the size of the front image (the size of the part that will be drawn)
        recalculateSize();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void LoadingBar::setValue(unsigned int value)
    {
        // Set the new value
        m_Value = value;

        // When the value is below the minimum or above the maximum then adjust it
        if (m_Value < m_Minimum)
            m_Value = m_Minimum;
        else if (m_Value > m_Maximum)
            m_Value = m_Maximum;

        // Recalculate the size of the front image (the size of the part that will be drawn)
        recalculateSize();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    unsigned int LoadingBar::incrementValue()
    {
        // When the value is still below the maximum then adjust it
        if (m_Value < m_Maximum)
        {
            ++m_Value;

            // Add the callback (if the user requested it)
            if (m_CallbackFunctions[ValueChanged].empty() == false)
            {
                m_Callback.trigger = ValueChanged;
                m_Callback.value   = static_cast<int>(m_Value);
                addCallback();
            }

            // Check if the loading bar is now full
            if (m_Value == m_Maximum)
            {
                // Add the callback (if the user requested it)
                if (m_CallbackFunctions[LoadingBarFull].empty() == false)
                {
                    m_Callback.trigger = LoadingBarFull;
                    m_Callback.value   = static_cast<int>(m_Value);
                    addCallback();
                }
            }

            // Recalculate the size of the front image (the size of the part that will be drawn)
            recalculateSize();
        }

        // return the new value
        return m_Value;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    unsigned int LoadingBar::getMinimum() const
    {
        return m_Minimum;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    unsigned int LoadingBar::getMaximum() const
    {
        return m_Maximum;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    unsigned int LoadingBar::getValue() const
    {
        return m_Value;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void LoadingBar::setText(const sf::String& text)
    {
        // Don't do anything when the loading bar wasn't loaded correctly
        if (m_Loaded == false)
            return;

        // Set the new text
        m_Text.setString(text);

        // Check if the text is auto sized
        if (m_TextSize == 0)
        {
            // Calculate a possible text size
            float size = m_Size.y * 0.85f;
            m_Text.setCharacterSize(static_cast<unsigned int>(size));
            m_Text.setCharacterSize(static_cast<unsigned int>(m_Text.getCharacterSize() - m_Text.getLocalBounds().top));

            // Make sure that the text isn't too width
            if (m_Text.getGlobalBounds().width > (m_Size.x * 0.8f))
            {
                // The text is too width, so make it smaller
                m_Text.setCharacterSize(static_cast<unsigned int>(size / (m_Text.getGlobalBounds().width / (m_Size.x * 0.8f))));
                m_Text.setCharacterSize(static_cast<unsigned int>(m_Text.getCharacterSize() - m_Text.getLocalBounds().top));
            }
        }
        else // When the text has a fixed size
        {
            // Set the text size
            m_Text.setCharacterSize(m_TextSize);
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    sf::String LoadingBar::getText() const
    {
        return m_Text.getString();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void LoadingBar::setTextFont(const sf::Font& font)
    {
        m_Text.setFont(font);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    const sf::Font* LoadingBar::getTextFont() const
    {
        return m_Text.getFont();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void LoadingBar::setTextColor(const sf::Color& color)
    {
        m_Text.setColor(color);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    const sf::Color& LoadingBar::getTextColor() const
    {
        return m_Text.getColor();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void LoadingBar::setTextSize(unsigned int size)
    {
        // Change the text size
        m_TextSize = size;

        // Call setText to reposition the text
        setText(m_Text.getString());
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    unsigned int LoadingBar::getTextSize() const
    {
        return m_Text.getCharacterSize();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void LoadingBar::setTransparency(unsigned char transparency)
    {
        ClickableWidget::setTransparency(transparency);

        m_TextureBack_L.sprite.setColor(sf::Color(255, 255, 255, m_Opacity));
        m_TextureBack_M.sprite.setColor(sf::Color(255, 255, 255, m_Opacity));
        m_TextureBack_R.sprite.setColor(sf::Color(255, 255, 255, m_Opacity));
        m_TextureFront_L.sprite.setColor(sf::Color(255, 255, 255, m_Opacity));
        m_TextureFront_M.sprite.setColor(sf::Color(255, 255, 255, m_Opacity));
        m_TextureFront_R.sprite.setColor(sf::Color(255, 255, 255, m_Opacity));

        m_Text.setColor(sf::Color(m_Text.getColor().r, m_Text.getColor().g, m_Text.getColor().b, m_Opacity));
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void LoadingBar::recalculateSize()
    {
        // Don't calculate anything when the loading bar wasn't loaded correctly
        if (m_Loaded == false)
            return;

        // Check if the image is split
        if (m_SplitImage)
        {
            // Get the bounds of the sprites
            sf::IntRect bounds_L = m_TextureFront_L.sprite.getTextureRect();
            sf::IntRect bounds_M = m_TextureFront_M.sprite.getTextureRect();
            sf::IntRect bounds_R = m_TextureFront_R.sprite.getTextureRect();

            bounds_L.width = m_TextureBack_L.sprite.getTextureRect().width;
            bounds_M.width = m_TextureBack_M.sprite.getTextureRect().width;
            bounds_R.width = m_TextureBack_R.sprite.getTextureRect().width;

            // Calculate the necessary sizes
            float totalWidth;
            float middleTextureWidth;
            float frontSize;

            // Check if the middle image is drawn
            if (m_TextureBack_L.getSize().x + m_TextureBack_R.getSize().x < m_Size.x)
            {
                totalWidth = bounds_L.width + bounds_M.width + bounds_R.width;
                middleTextureWidth = totalWidth - (m_TextureBack_L.getSize().x + m_TextureBack_R.getSize().x);
            }
            else // The loading bar is too small
            {
                totalWidth = bounds_L.width + bounds_R.width;
                middleTextureWidth = 0;
            }

            // Only change the width when not dividing by zero
            if ((m_Maximum - m_Minimum) > 0)
                frontSize = totalWidth * ((m_Value - m_Minimum) / static_cast<float>(m_Maximum - m_Minimum));
            else
                frontSize = totalWidth;

            // Check if a part of the left piece should be visible
            if (frontSize > 0)
            {
                // Check if a piece of the middle part should be drawn
                if (frontSize > m_TextureBack_L.getSize().x)
                {
                    // Check if a piece of the right part should be drawn
                    if (frontSize > m_TextureBack_L.getSize().x + middleTextureWidth)
                    {
                        // Check if the bar is not full
                        if (frontSize < totalWidth)
                            bounds_R.width = static_cast<int>(frontSize - ((m_TextureBack_L.getSize().x) + middleTextureWidth));
                    }
                    else // Only a part of the middle image should be drawn
                    {
                        bounds_M.width = static_cast<int>((frontSize - (m_TextureBack_L.getSize().x)) / middleTextureWidth * m_TextureBack_M.getSize().x);
                        bounds_R.width = 0;
                    }
                }
                else // Only a part of the left piece should be drawn
                {
                    bounds_L.width = frontSize;
                    bounds_M.width = 0;
                    bounds_R.width = 0;
                }
            }
            else // Nothing should be drawn
            {
                bounds_L.width = 0;
                bounds_M.width = 0;
                bounds_R.width = 0;
            }

            m_TextureFront_L.sprite.setTextureRect(bounds_L);
            m_TextureFront_M.sprite.setTextureRect(bounds_M);
            m_TextureFront_R.sprite.setTextureRect(bounds_R);
        }
        else // The image is not split
        {
            // Calculate the size of the front sprite
            sf::IntRect frontBounds(m_TextureFront_M.sprite.getTextureRect());

            // Only change the width when not dividing by zero
            if ((m_Maximum - m_Minimum) > 0)
                frontBounds.width = static_cast<int>(m_TextureBack_M.getSize().x * ((m_Value - m_Minimum) / static_cast<float>(m_Maximum - m_Minimum)));
            else
                frontBounds.width = static_cast<int>(m_TextureBack_M.getSize().x);

            // Set the size of the front image
            m_TextureFront_M.sprite.setTextureRect(frontBounds);
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void LoadingBar::initialize(tgui::Container *const parent)
    {
        m_Parent = parent;
        m_Text.setFont(m_Parent->getGlobalFont());
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void LoadingBar::draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        // Don't draw when the loading bar wasn't loaded correctly
        if (m_Loaded == false)
            return;

        // Apply the transformation
        states.transform *= getTransform();

        // Remember the current transformation
        sf::Transform oldTransform = states.transform;

        // Check if the image is split
        if (m_SplitImage)
        {
            // Get the scale the images
            float scalingY = m_Size.y / m_TextureBack_M.getSize().y;

            // Scale the image
            states.transform.scale(scalingY, scalingY);

            // Draw the left image of the loading bar
            target.draw(m_TextureBack_L, states);
            target.draw(m_TextureFront_L, states);

            // Check if the middle image may be drawn
            if ((scalingY * (m_TextureBack_L.getSize().x + m_TextureBack_R.getSize().x)) < m_Size.x)
            {
                // Put the middle image on the correct position
                states.transform.translate(m_TextureBack_L.sprite.getGlobalBounds().width, 0);

                // Calculate the scale for our middle image
                float scaleX = (m_Size.x - ((m_TextureBack_L.getSize().x + m_TextureBack_R.getSize().x) * scalingY)) / m_TextureBack_M.getSize().x;

                // Set the scale for the middle image
                states.transform.scale(scaleX / scalingY, 1);

                // Draw the middle image
                target.draw(m_TextureBack_M, states);
                target.draw(m_TextureFront_M, states);

                // Put the right image on the correct position
                states.transform.translate(m_TextureBack_M.sprite.getGlobalBounds().width, 0);

                // Set the scale for the right image
                states.transform.scale(scalingY / scaleX, 1);

                // Draw the right image
                target.draw(m_TextureBack_R, states);
                target.draw(m_TextureFront_R, states);
            }
            else // The loading bar isn't width enough, we will draw it at minimum size
            {
                // Put the right image on the correct position
                states.transform.translate(m_TextureBack_L.sprite.getGlobalBounds().width, 0);

                // Draw the right image
                target.draw(m_TextureBack_R, states);
                target.draw(m_TextureFront_R, states);
            }
        }
        else // The image is not split
        {
            // Scale the image
            states.transform.scale(m_Size.x / m_TextureBack_M.getSize().x, m_Size.y / m_TextureBack_M.getSize().y);

            // Draw the loading bar
            target.draw(m_TextureBack_M, states);
            target.draw(m_TextureFront_M, states);
        }

        // Check if there is a text to draw
        if (m_Text.getString().isEmpty() == false)
        {
            // Reset the transformations
            states.transform = oldTransform;

            // Get the current size of the text, so that we can recalculate the position
            sf::FloatRect rect = m_Text.getGlobalBounds();

            // Calculate the new position for the text
            rect.left = (m_Size.x - rect.width) * 0.5f - rect.left;
            rect.top = (m_Size.y - rect.height) * 0.5f - rect.top;

            // Set the new position
            states.transform.translate(std::floor(rect.left + 0.5f), std::floor(rect.top + 0.5f));

            // Draw the text
            target.draw(m_Text, states);
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
