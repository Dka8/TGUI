/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// TGUI - Texus's Graphical User Interface
// Copyright (C) 2012 Bruno Van de Velde (VDV_B@hotmail.com)
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


#include <TGUI/TGUI.hpp>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace tgui
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    Scrollbar::Scrollbar() :
    autoHide            (true),
    m_LowValue          (0),
    m_TextureArrowNormal(NULL),
    m_TextureArrowHover (NULL)
    {
        m_ObjectType = scrollbar;
        m_DraggableObject = true;

        m_Maximum = 0;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    Scrollbar::Scrollbar(const Scrollbar& copy) :
    Slider    (copy),
    autoHide  (copy.autoHide),
    m_LowValue(copy.m_LowValue)
    {
        // Copy the textures
        if (TGUI_TextureManager.copyTexture(copy.m_TextureArrowNormal, m_TextureArrowNormal)) m_SpriteArrowNormal.setTexture(*m_TextureArrowNormal);
        if (TGUI_TextureManager.copyTexture(copy.m_TextureArrowHover, m_TextureArrowHover))   m_SpriteArrowHover.setTexture(*m_TextureArrowHover);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    Scrollbar::~Scrollbar()
    {
        if (m_TextureArrowNormal != NULL) TGUI_TextureManager.removeTexture(m_TextureArrowNormal);
        if (m_TextureArrowHover != NULL)  TGUI_TextureManager.removeTexture(m_TextureArrowHover);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    Scrollbar& Scrollbar::operator= (const Scrollbar& right)
    {
        // Make sure it is not the same object
        if (this != &right)
        {
            Scrollbar temp(right);
            this->Slider::operator=(right);

            std::swap(autoHide,             temp.autoHide);
            std::swap(m_LowValue,           temp.m_LowValue);
            std::swap(m_TextureArrowNormal, temp.m_TextureArrowNormal);
            std::swap(m_TextureArrowHover,  temp.m_TextureArrowHover);
            std::swap(m_SpriteArrowNormal,  temp.m_SpriteArrowNormal);
            std::swap(m_SpriteArrowHover,   temp.m_SpriteArrowHover);
        }

        return *this;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    Scrollbar* Scrollbar::clone()
    {
        return new Scrollbar(*this);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    bool Scrollbar::load(const std::string pathname)
    {
        // When everything is loaded successfully, this will become true.
        m_Loaded = false;

        // Make sure that the pathname isn't empty
        if (pathname.empty())
            return false;

        // Store the pathname
        m_LoadedPathname = pathname;

        // When the pathname does not end with a "/" then we will add it
        if (m_LoadedPathname[m_LoadedPathname.length()-1] != '/')
            m_LoadedPathname.push_back('/');

        // Open the info file
        InfoFileParser infoFile;
        if (infoFile.openFile(m_LoadedPathname + "info.txt") == false)
        {
            TGUI_OUTPUT((((std::string("TGUI: Failed to open ")).append(m_LoadedPathname)).append("info.txt")).c_str());
            return false;
        }

        std::string property;
        std::string value;

        std::string imageExtension = "png";

        // Read untill the end of the file
        while (infoFile.readProperty(property, value))
        {
            // Check what the property is
            if (property.compare("phases") == 0)
            {
                // Get and store the different phases
                extractPhases(value);
            }
            else if (property.compare("extension") == 0)
            {
                imageExtension = value;
            }
            else if (property.compare("verticalscroll") == 0)
            {
                m_VerticalImage = !!atoi(value.c_str());
                verticalScroll = m_VerticalImage;
            }
        }

        // Close the info file
        infoFile.closeFile();

        // Remove all textures when they were loaded before
        if (m_TextureTrackNormal_L != NULL) TGUI_TextureManager.removeTexture(m_TextureTrackNormal_L);
        if (m_TextureTrackHover_L != NULL)  TGUI_TextureManager.removeTexture(m_TextureTrackHover_L);
        if (m_TextureTrackNormal_M != NULL) TGUI_TextureManager.removeTexture(m_TextureTrackNormal_M);
        if (m_TextureTrackHover_M != NULL)  TGUI_TextureManager.removeTexture(m_TextureTrackHover_M);
        if (m_TextureTrackNormal_R != NULL) TGUI_TextureManager.removeTexture(m_TextureTrackNormal_R);
        if (m_TextureTrackHover_R != NULL)  TGUI_TextureManager.removeTexture(m_TextureTrackHover_R);

        if (m_TextureThumbNormal != NULL) TGUI_TextureManager.removeTexture(m_TextureThumbNormal);
        if (m_TextureArrowNormal != NULL) TGUI_TextureManager.removeTexture(m_TextureArrowNormal);
        if (m_TextureArrowHover != NULL)  TGUI_TextureManager.removeTexture(m_TextureArrowHover);

        // load the required textures
        if ((TGUI_TextureManager.getTexture(m_LoadedPathname + "Track_Normal." + imageExtension, m_TextureTrackNormal_M))
         && (TGUI_TextureManager.getTexture(m_LoadedPathname + "Thumb_Normal." + imageExtension, m_TextureThumbNormal))
         && (TGUI_TextureManager.getTexture(m_LoadedPathname + "Arrow_Normal." + imageExtension, m_TextureArrowNormal)))
        {
            m_SpriteTrackNormal_M.setTexture(*m_TextureTrackNormal_M, true);
            m_SpriteThumbNormal.setTexture(*m_TextureThumbNormal, true);
            m_SpriteArrowNormal.setTexture(*m_TextureArrowNormal, true);
        }
        else
            return false;

        bool error = false;

        // load the optional textures

        if (m_ObjectPhase & ObjectPhase_Hover)
        {
            if ((TGUI_TextureManager.getTexture(m_LoadedPathname + "Track_Hover." + imageExtension, m_TextureTrackHover_M))
             && (TGUI_TextureManager.getTexture(m_LoadedPathname + "Thumb_Hover." + imageExtension, m_TextureThumbHover))
             && (TGUI_TextureManager.getTexture(m_LoadedPathname + "Arrow_Hover." + imageExtension, m_TextureArrowHover)))
             {
                 m_SpriteTrackHover_M.setTexture(*m_TextureTrackHover_M, true);
                 m_SpriteThumbHover.setTexture(*m_TextureThumbHover, true);
                 m_SpriteArrowHover.setTexture(*m_TextureArrowHover, true);
             }
            else
                error = true;
        }

        // When there is no error we will return true
        m_Loaded = !error;
        return !error;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Scrollbar::setMinimum(unsigned int minimum)
    {
        // Do nothing. The minimum may not be changed.
        TGUI_UNUSED_PARAM(minimum);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Scrollbar::setValue(unsigned int value)
    {
        // Set the new value
        m_Value = value;

        // When the value is above the maximum then adjust it
        if (m_Value > m_Maximum - m_LowValue)
            m_Value = m_Maximum - m_LowValue;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Scrollbar::setLowValue(unsigned int lowValue)
    {
        // Set the new value
        m_LowValue = lowValue;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    unsigned int Scrollbar::getLowValue()
    {
        return m_LowValue;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    bool Scrollbar::mouseOnObject(float x, float y)
    {
        // Don't do anything when the scrollbar wasn't loaded correctly
        if (m_Loaded == false)
            return false;

        // Don't make any calculations when no scrollbar is needed
        if (m_Maximum <= m_LowValue)
            return false;

        // Check if the mouse is on top of the scrollbar
        if (getTransform().transformRect(sf::FloatRect(0, 0, static_cast<float>(getSize().x), static_cast<float>(getSize().y))).contains(x, y))
        {
            // Get the current position and scale
            Vector2f position = getPosition();
            Vector2f curScale = getScale();

            // Calculate the track size, thumb size and thumb position
            float trackWidth, trackHeight;
            float thumbWidth, thumbHeight;
            float thumbLeft,  thumbTop;

            // The size is different when the image is rotated
            if (m_VerticalImage == verticalScroll)
            {
                trackWidth = m_TextureTrackNormal_M->getSize().x * curScale.x;
                trackHeight = m_TextureTrackNormal_M->getSize().y * curScale.y;
            }
            else
            {
                trackWidth = m_TextureTrackNormal_M->getSize().y * curScale.x;
                trackHeight = m_TextureTrackNormal_M->getSize().x * curScale.y;
            }

            // The size is different when the image is rotated
            if (m_VerticalImage == verticalScroll)
            {
                thumbWidth = static_cast<float>(m_TextureThumbNormal->getSize().x);
                thumbHeight = static_cast<float>(m_TextureThumbNormal->getSize().y);
            }
            else
            {
                thumbWidth = static_cast<float>(m_TextureThumbNormal->getSize().y);
                thumbHeight = static_cast<float>(m_TextureThumbNormal->getSize().x);
            }

            // The scaling depends on how the scrollbar lies
            if (verticalScroll)
            {
                float realTrackHeight;

                // Check if the arrows are drawn at full size
                if (trackHeight > 2 * m_TextureArrowNormal->getSize().y * curScale.x)
                {
                    // Calculate the track height
                    realTrackHeight = trackHeight - (2 * m_TextureArrowNormal->getSize().y * curScale.x);
                    thumbHeight = ((static_cast<float>(m_LowValue) / m_Maximum) * realTrackHeight);

                    // Calculate the top position of the thumb
                    thumbTop = (m_TextureArrowNormal->getSize().y * curScale.x) + ((static_cast<float>(m_Value) / (m_Maximum - m_LowValue)) * (realTrackHeight - thumbHeight));
                }
                else // The arrows are not drawn at full size
                {
                    // Calculate the real track height (without the arrows)
                    realTrackHeight = trackHeight - (2 * m_TextureArrowNormal->getSize().y * curScale.y);
                    thumbHeight = ((static_cast<float>(m_LowValue) / m_Maximum) * realTrackHeight);

                    // Calculate the top position of the thumb
                    thumbTop = (m_TextureArrowNormal->getSize().y * curScale.y) + ((static_cast<float>(m_Value) / (m_Maximum - m_LowValue)) * (realTrackHeight - thumbHeight));
                }

                // Calculate the thumb width and left position
                thumbWidth *= curScale.x;
                thumbLeft = (trackWidth - thumbWidth) * 0.5f;
            }
            else // The scrollbar lies horizontal
            {
                float realTrackWidth;

                // Check if the arrows are drawn at full size
                if (trackWidth * curScale.x > 2 * m_TextureArrowNormal->getSize().x * curScale.y)
                {
                    // Calculate the track height
                    realTrackWidth = trackWidth - (2 * m_TextureArrowNormal->getSize().y * curScale.y);
                    thumbWidth = ((static_cast<float>(m_LowValue) / m_Maximum) * realTrackWidth);

                    // Calculate the left position of the thumb
                    thumbLeft = (m_TextureArrowNormal->getSize().x * curScale.y) + ((static_cast<float>(m_Value) / (m_Maximum - m_LowValue)) * (realTrackWidth - thumbWidth));
                }
                else
                {
                    // Calculate the track height
                    realTrackWidth = trackWidth - (2 * m_TextureArrowNormal->getSize().y * curScale.x);
                    thumbWidth = ((static_cast<float>(m_LowValue) / m_Maximum) * realTrackWidth);

                    // Calculate the left position of the thumb
                    thumbLeft = (m_TextureArrowNormal->getSize().x * curScale.x) + ((static_cast<float>(m_Value) / (m_Maximum - m_LowValue)) * (realTrackWidth - thumbWidth));
                }

                // Calculate the thumb height and top position
                thumbHeight *= curScale.y;
                thumbTop = (trackHeight - thumbHeight) * 0.5f;
            }

            // Check if the mouse is on top of the thumb
            if (sf::FloatRect(position.x + thumbLeft, position.y + thumbTop, thumbWidth, thumbHeight).contains(x, y))
            {
                if (m_MouseDown == false)
                {
                    m_MouseDownOnThumbPos.x = x - position.x - thumbLeft;
                    m_MouseDownOnThumbPos.y = y - position.y - thumbTop;
                }

                m_MouseDownOnThumb = true;
                return true;
            }
            else // The mouse is not on top of the thumb
                m_MouseDownOnThumb = false;

            return true;
        }

        // The mouse is not on top of the scrollbar
        m_MouseHover = false;
        return false;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Scrollbar::leftMousePressed(float x, float y)
    {
        m_MouseDown = true;

        // Refresh the value
        mouseMoved(x, y);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Scrollbar::leftMouseReleased(float x, float y)
    {
        TGUI_UNUSED_PARAM(x);

        // Check ig the scrollbar was really clicked
        if (m_MouseDown)
        {
            // Remember the current value
            unsigned int oldValue = m_Value;

            // Check in which direction the scrollbar lies
            if (verticalScroll)
            {
                // Calculate the track height
                float trackHeight;

                if (m_VerticalImage == verticalScroll)
                    trackHeight = m_TextureTrackNormal_M->getSize().y * getScale().y;
                else
                    trackHeight = m_TextureTrackNormal_M->getSize().x * getScale().y;

                // Check if the arrows are drawn at full size
                if (trackHeight > 2 * m_TextureArrowNormal->getSize().y * getScale().x)
                {
                    // Check if you clicked on the top arrow
                    if (y < getPosition().y + (m_TextureArrowNormal->getSize().y * getScale().x))
                    {
                        if (m_Value > 0)
                            --m_Value;
                    }

                    // Check if you clicked the down arrow
                    else if (y > getPosition().y + trackHeight - (m_TextureArrowNormal->getSize().y * getScale().x))
                    {
                        if (m_Value < (m_Maximum - m_LowValue))
                            ++m_Value;
                    }
                }
                else // The arrows are not drawn at full size
                {
                    // Check if you clicked on the top arrow
                    if (y < getPosition().y + (m_TextureArrowNormal->getSize().y * getScale().y))
                    {
                        if (m_Value > 0)
                            --m_Value;
                    }

                    // Check if you clicked the down arrow
                    else if (y > getPosition().y + trackHeight - (m_TextureArrowNormal->getSize().y * getScale().y))
                    {
                        if (m_Value < (m_Maximum - m_LowValue))
                            ++m_Value;
                    }
                }
            }
            else // The scrollbar lies horizontal
            {
                // Calculate the track width
                float trackWidth;

                if (m_VerticalImage == verticalScroll)
                    trackWidth = m_TextureTrackNormal_M->getSize().x * getScale().x;
                else
                    trackWidth = m_TextureTrackNormal_M->getSize().y * getScale().x;

                // Check if the arrows are drawn at full size
                if (trackWidth > 2 * m_TextureArrowNormal->getSize().x * getScale().y)
                {
                    // Check if you clicked on the left arrow
                    if (x < getPosition().x + (m_TextureArrowNormal->getSize().y * getScale().y))
                    {
                        if (m_Value > 0)
                            --m_Value;
                    }

                    // Check if you clicked the right arrow
                    else if (x > getPosition().x + trackWidth - (m_TextureArrowNormal->getSize().y * getScale().y))
                    {
                        if (m_Value < (m_Maximum - m_LowValue))
                            ++m_Value;
                    }
                }
                else // The arrows are not drawn at full size
                {
                    // Check if you clicked on the left arrow
                    if (x < getPosition().x + (m_TextureArrowNormal->getSize().y * getScale().x))
                    {
                        if (m_Value > 0)
                            --m_Value;
                    }

                    // Check if you clicked the right arrow
                    else if (x > getPosition().x + trackWidth - (m_TextureArrowNormal->getSize().y * getScale().x))
                    {
                        if (m_Value < (m_Maximum - m_LowValue))
                            ++m_Value;
                    }
                }
            }

            // The mouse is no longer down
            m_MouseDown = false;

            // Add the callback (if the user requested it and the value has changed)
            if ((callbackID > 0) && (oldValue != m_Value))
            {
                Callback callback;
                callback.object     = this;
                callback.callbackID = callbackID;
                callback.trigger    = Callback::valueChanged;
                callback.value      = m_Value;
                m_Parent->addCallback(callback);
            }
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Scrollbar::mouseMoved(float x, float y)
    {
        TGUI_UNUSED_PARAM(x);

        // If the scrollbar wasn't loaded then do nothing
        if (m_Loaded == false)
            return;

        m_MouseHover = true;

        // Get the current position and scale
        Vector2f position = getPosition();
        Vector2f curScale = getScale();

        // Check if the mouse button is down
        if (m_MouseDown)
        {
            // Remember the current value
            unsigned int oldValue = m_Value;

            // Check in which direction the scrollbar lies
            if (verticalScroll)
            {
                // Calculate the track height
                float trackHeight;

                if (m_VerticalImage == verticalScroll)
                    trackHeight = m_TextureTrackNormal_M->getSize().y * curScale.y;
                else
                    trackHeight = m_TextureTrackNormal_M->getSize().x * curScale.y;

                // Calculate the arrow height
                float arrowHeight;

                if (trackHeight > 2 * m_TextureArrowNormal->getSize().y * getScale().x)
                    arrowHeight = m_TextureArrowNormal->getSize().y * curScale.x;
                else
                    arrowHeight = m_TextureArrowNormal->getSize().y * curScale.y;

                // Check if the thumb is being dragged
                if (m_MouseDownOnThumb)
                {
                    // Set the new value
                    if ((y - m_MouseDownOnThumbPos.y - position.y - arrowHeight) > 0)
                    {
                        // Calculate the new value
                        unsigned value = static_cast <unsigned int> ((((y - m_MouseDownOnThumbPos.y - position.y - arrowHeight)
                                                                       / (trackHeight - (2 * arrowHeight))) * m_Maximum) + 0.5f);

                        // If the value isn't too high then change it
                        if (value <= (m_Maximum - m_LowValue))
                            setValue(value);
                        else
                            setValue(m_Maximum - m_LowValue);
                    }
                    else // The mouse was above the scrollbar
                        setValue(0);
                }
                else // The click occured on the track
                {
                    // If the position is positive then calculate the correct value
                    if (y > position.y + arrowHeight)
                    {
                        // Make sure that you didn't click on the down arrow
                        if (y <= position.y + trackHeight - arrowHeight)
                        {
                            // Calculate the exact position (a number between 0 and maximum)
                            float value = (((y - position.y - arrowHeight)
                                             / (trackHeight - (2 * arrowHeight))) * m_Maximum);

                            // Check if you clicked above the thumb
                            if (value <= m_Value)
                            {
                                float subtractValue = m_LowValue / 3.0f;

                                // Try to place the thumb on 2/3 of the clicked position
                                if (value >= subtractValue)
                                    setValue(static_cast<unsigned int>(value - subtractValue + 0.5f));
                                else
                                    setValue(0);
                            }
                            else // The click occured below the thumb
                            {
                                float subtractValue = m_LowValue * 2.0f / 3.0f;

                                // Try to place the thumb on 2/3 of the clicked position
                                if (value <= (m_Maximum - m_LowValue + subtractValue))
                                    setValue(static_cast<unsigned int>(value - subtractValue + 0.5f));
                                else
                                    setValue(m_Maximum - m_LowValue);
                            }

                            mouseOnObject(x, y);
                        }
                    }
                }
            }
            else // the scrollbar lies horizontal
            {
                // Calculate the track width
                float trackWidth;

                if (m_VerticalImage == verticalScroll)
                    trackWidth = m_TextureTrackNormal_M->getSize().x * curScale.x;
                else
                    trackWidth = m_TextureTrackNormal_M->getSize().y * curScale.x;

                // Calculate the arrow width
                float arrowWidth;

                if (trackWidth > 2 * m_TextureArrowNormal->getSize().x * getScale().y)
                    arrowWidth = m_TextureArrowNormal->getSize().y * curScale.y;
                else
                    arrowWidth = m_TextureArrowNormal->getSize().y * curScale.x;

                // Check if the thumb is being dragged
                if (m_MouseDownOnThumb)
                {
                    // Set the new value
                    if ((x - m_MouseDownOnThumbPos.x - position.x - arrowWidth) > 0)
                    {
                        // Calculate the new value
                        unsigned value = static_cast <unsigned int> ((((x - m_MouseDownOnThumbPos.x - position.x - arrowWidth)
                                                                       / (trackWidth - (2 * arrowWidth))) * m_Maximum) + 0.5f);

                        // Set the new value
                        if (value <= (m_Maximum - m_LowValue))
                            setValue(value);
                        else
                            setValue(m_Maximum - m_LowValue);
                    }
                    else // The mouse was on the left of the scrollbar
                        setValue(0);
                }
                else // The click occured on the track
                {
                    // If the position is positive then calculate the correct value
                    if (x > position.x + arrowWidth)
                    {
                        // Make sure that you didn't click on the left arrow
                        if (x <= position.x + trackWidth - arrowWidth)
                        {
                            // Calculate the exact position (a number between 0 and maximum)
                            float value = (((x - position.x - arrowWidth)
                                             / (trackWidth - (2 * arrowWidth))) * m_Maximum);

                            // Check if you clicked above the thumb
                            if (value <= m_Value)
                            {
                                float subtractValue = m_LowValue / 3.0f;

                                // Try to place the thumb on 2/3 of the clicked position
                                if (value >= subtractValue)
                                    setValue(static_cast<unsigned int>(value - subtractValue + 0.5f));
                                else
                                    setValue(0);
                            }
                            else // The click occured below the thumb
                            {
                                float subtractValue = m_LowValue * 2.0f / 3.0f;

                                // Try to place the thumb on 2/3 of the clicked position
                                if (value <= (m_Maximum - m_LowValue + subtractValue))
                                    setValue(static_cast<unsigned int>(value - subtractValue + 0.5f));
                                else
                                    setValue(m_Maximum - m_LowValue);
                            }

                            mouseOnObject(x, y);
                        }
                    }
                }
            }

            // Add the callback (if the user requested it and the value has changed)
            if ((callbackID > 0) && (oldValue != m_Value))
            {
                Callback callback;
                callback.object     = this;
                callback.callbackID = callbackID;
                callback.trigger    = Callback::valueChanged;
                callback.value      = m_Value;
                m_Parent->addCallback(callback);
            }
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Scrollbar::draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        // If the scrollbar wasn't loaded then don't draw it
        if (m_Loaded == false)
            return;

        // Don't draw the loading bar when it isn't needed
        if ((autoHide == true) && (m_Maximum <= m_LowValue))
            return;

        float trackWidth, trackHeight;
        float thumbWidth, thumbHeight;

        // Get the current position and scale
        Vector2f position = getPosition();
        Vector2f curScale = getScale();

        // Remember the current transformation
        sf::Transform oldTransform = states.transform;

        // Adjust the transformation
        states.transform *= getTransform();

        // It is possible that the image is not drawn in the same direction than the loaded image
        if ((m_VerticalImage == true) && (verticalScroll == false))
        {
            // Set the track rotation
            states.transform.rotate(-90, m_TextureTrackNormal_M->getSize().x * 0.5f, m_TextureTrackNormal_M->getSize().x * 0.5f);

            // Calculate the track size
            trackWidth = static_cast<float>(m_TextureTrackNormal_M->getSize().y);
            trackHeight = static_cast<float>(m_TextureTrackNormal_M->getSize().x);

            // Calculate the thumb size
            thumbWidth = static_cast<float>(m_TextureThumbNormal->getSize().y);
            thumbHeight = static_cast<float>(m_TextureThumbNormal->getSize().x);

        }
        else if ((m_VerticalImage == false) && (verticalScroll == true))
        {
            // Set the track rotation
            states.transform.rotate(90, m_TextureTrackNormal_M->getSize().y * 0.5f, m_TextureTrackNormal_M->getSize().y * 0.5f);

            // Calculate the track size
            trackWidth = static_cast<float>(m_TextureTrackNormal_M->getSize().y);
            trackHeight = static_cast<float>(m_TextureTrackNormal_M->getSize().x);

            // Calculate the thumb size
            thumbWidth = static_cast<float>(m_TextureThumbNormal->getSize().y);
            thumbHeight = static_cast<float>(m_TextureThumbNormal->getSize().x);
        }
        else
        {
            // Calculate the track size
            trackWidth = static_cast<float>(m_TextureTrackNormal_M->getSize().x);
            trackHeight = static_cast<float>(m_TextureTrackNormal_M->getSize().y);

            // Calculate the thumb size
            thumbWidth = static_cast<float>(m_TextureThumbNormal->getSize().x);
            thumbHeight = static_cast<float>(m_TextureThumbNormal->getSize().y);
        }

        // Draw the normal track image
        target.draw(m_SpriteTrackNormal_M, states);

        // When the mouse is on top of the scrollbar then draw the hover image
        if ((m_MouseHover) && (m_ObjectPhase & ObjectPhase_Hover))
            target.draw(m_SpriteTrackHover_M, states);


        // Reset the transformation (in case there was any rotation)
        states.transform = oldTransform;

        // Adjust the transformation
        states.transform.translate(position.x, position.y);

        // The calculation depends on the direction of the scrollbar
        if (verticalScroll)
        {
            // Check if the arrows can be drawn at full size
            if (trackHeight * curScale.y > 2 * m_TextureArrowNormal->getSize().y * curScale.x)
            {
                // Scale the arrow
                states.transform.scale(curScale.x, curScale.x);

                // Draw the first normal arrow
                target.draw(m_SpriteArrowNormal, states);

                // When the mouse is on top of the scrollbar then draw the hover image
                if ((m_MouseHover) && (m_ObjectPhase & ObjectPhase_Hover))
                    target.draw(m_SpriteArrowHover, states);

                // Calculate the real track height (height without the arrows)
                float realTrackHeight = (trackHeight * curScale.y) - (2 * m_TextureArrowNormal->getSize().y * curScale.x);

                // Calculate the scaling factor
                float scaleY;
                if ((autoHide == false) && (m_Maximum <= m_LowValue))
                    scaleY = realTrackHeight / (thumbHeight * curScale.y);
                else
                    scaleY = (((static_cast<float>(m_LowValue) / m_Maximum)) * realTrackHeight) / (thumbHeight * curScale.x);

                // Set the thumb on the correct position
                states.transform.translate(0, m_TextureArrowNormal->getSize().y + (m_Value * ((realTrackHeight / curScale.x) / m_Maximum)));

                // Scale the image
                states.transform.scale(1, scaleY);

                // Draw the normal thumb image
                target.draw(m_SpriteThumbNormal, states);

                // When the mouse is on top of the scrollbar then draw the hover image
                if ((m_MouseHover) && (m_ObjectPhase & ObjectPhase_Hover))
                    target.draw(m_SpriteThumbHover, states);

                // Reset the transformation
                states.transform = oldTransform;

                // Change the position of the second arrow
                states.transform.translate(position.x, position.y + (trackHeight * curScale.y));

                // Set the scale of the arrow
                states.transform.scale(curScale.x, -curScale.x);
            }
            else // The arrows can't be drawn at full size
            {
                // Scale the arrow
                states.transform.scale(curScale.x, curScale.y);

                // Draw the first normal arrow
                target.draw(m_SpriteArrowNormal, states);

                // When the mouse is on top of the scrollbar then draw the hover image
                if ((m_MouseHover) && (m_ObjectPhase & ObjectPhase_Hover))
                    target.draw(m_SpriteArrowHover, states);

                // Calculate the real track height (height without the arrows)
                float realTrackHeight = (trackHeight * curScale.y) - (2 * m_TextureArrowNormal->getSize().y * curScale.y);

                // Calculate the scaling factor
                float scaleY;
                if ((autoHide == false) && (m_Maximum <= m_LowValue))
                    scaleY = realTrackHeight / (thumbHeight * curScale.y);
                else
                    scaleY = (((static_cast<float>(m_LowValue) / m_Maximum)) * realTrackHeight) / (thumbHeight * curScale.y);

                // Set the thumb on the correct position
                states.transform.translate(0, m_TextureArrowNormal->getSize().y + (m_Value * ((realTrackHeight / curScale.y) / m_Maximum)));

                // Scale the image
                states.transform.scale(1, scaleY);

                // Draw the normal thumb image
                target.draw(m_SpriteThumbNormal, states);

                // When the mouse is on top of the scrollbar then draw the hover image
                if ((m_MouseHover) && (m_ObjectPhase & ObjectPhase_Hover))
                    target.draw(m_SpriteThumbHover, states);

                // Reset the transformation
                states.transform = oldTransform;

                // Change the position of the second arrow
                states.transform.translate(position.x, position.y + (trackHeight * curScale.y));

                // Set the scale of the arrow
                states.transform.scale(curScale.x, -curScale.y);
            }

            // Draw the second arrow
            target.draw(m_SpriteArrowNormal, states);

            // When the mouse is on top of the scrollbar then draw the hover image
            if ((m_MouseHover) && (m_ObjectPhase & ObjectPhase_Hover))
                target.draw(m_SpriteArrowHover, states);
        }
        else
        {
            // Check if the arrows can be drawn at full size
            if (trackWidth * curScale.x > 2 * m_TextureArrowNormal->getSize().x * curScale.y)
            {
                // Scale the arrow
                states.transform.scale(curScale.y, curScale.y);

                // Rotate the arrow
                states.transform.rotate(-90, m_TextureArrowNormal->getSize().y * 0.5f, m_TextureArrowNormal->getSize().y * 0.5f);

                // Draw the first normal arrow
                target.draw(m_SpriteArrowNormal, states);

                // When the mouse is on top of the scrollbar then draw the hover image
                if ((m_MouseHover) && (m_ObjectPhase & ObjectPhase_Hover))
                    target.draw(m_SpriteArrowHover, states);

                // Calculate the real track width (width without the arrows)
                float realTrackWidth = (trackWidth * curScale.x) - (2 * m_TextureArrowNormal->getSize().y * curScale.y);

                // Calculate the scaling factor
                float scaleX;
                if ((autoHide == false) && (m_Maximum <= m_LowValue))
                    scaleX = realTrackWidth / (thumbWidth * curScale.y);
                else
                    scaleX = (((static_cast<float>(m_LowValue) / m_Maximum)) * realTrackWidth) / (thumbWidth * curScale.y);

                // Set the thumb on the correct position
                states.transform.translate(0, m_TextureArrowNormal->getSize().y + m_Value * ((realTrackWidth / curScale.y) / m_Maximum));

                // Scale the image
                states.transform.scale(1, scaleX);

                // Draw the normal thumb image
                target.draw(m_SpriteThumbNormal, states);

                // When the mouse is on top of the scrollbar then draw the hover image
                if ((m_MouseHover) && (m_ObjectPhase & ObjectPhase_Hover))
                    target.draw(m_SpriteThumbHover, states);

                // Reset the transformation
                states.transform = oldTransform;

                // Change the position of the second arrow
                states.transform.translate(position.x + (trackWidth * curScale.x), position.y);

                // Set the scale of the arrow
                states.transform.scale(-curScale.y, curScale.y);
            }
            else // The arrows can't be drawn at full size
            {
                // Set the scale
                states.transform.scale(curScale.x, curScale.y);

                // Rotate the arrow
                states.transform.rotate(-90, m_TextureArrowNormal->getSize().y * 0.5f, m_TextureArrowNormal->getSize().y * 0.5f);

                // Draw the first normal arrow
                target.draw(m_SpriteArrowNormal, states);

                // When the mouse is on top of the scrollbar then draw the hover image
                if ((m_MouseHover) && (m_ObjectPhase & ObjectPhase_Hover))
                    target.draw(m_SpriteArrowHover, states);

                // Calculate the real track width (width without the arrows)
                float realTrackWidth = (trackWidth * curScale.x) - (2 * m_TextureArrowNormal->getSize().y * curScale.x);

                // Calculate the scaling factor
                float scaleX;
                if ((autoHide == false) && (m_Maximum <= m_LowValue))
                    scaleX = realTrackWidth / (thumbWidth * curScale.x);
                else
                    scaleX = (((static_cast<float>(m_LowValue) / m_Maximum)) * realTrackWidth) / (thumbWidth * curScale.x);

                // Set the thumb on the correct position
                states.transform.translate(0, m_TextureArrowNormal->getSize().y + m_Value * ((realTrackWidth / curScale.x) / m_Maximum));

                // Scale the image
                states.transform.scale(1, scaleX);

                // Draw the normal thumb image
                target.draw(m_SpriteThumbNormal, states);

                // When the mouse is on top of the scrollbar then draw the hover image
                if ((m_MouseHover) && (m_ObjectPhase & ObjectPhase_Hover))
                    target.draw(m_SpriteThumbHover, states);

                // Reset the transformation
                states.transform = oldTransform;

                // Change the position of the second arrow
                states.transform.translate(position.x + (trackWidth * curScale.x), position.y);

                // Set the scale of the arrow
                states.transform.scale(-curScale.x, curScale.y);
            }

            // Rotate the arrow
            states.transform.rotate(-90, m_TextureArrowNormal->getSize().y * 0.5f, m_TextureArrowNormal->getSize().y * 0.5f);

            // Draw the second arrow
            target.draw(m_SpriteArrowNormal, states);

            // When the mouse is on top of the scrollbar then draw the hover image
            if ((m_MouseHover) && (m_ObjectPhase & ObjectPhase_Hover))
                target.draw(m_SpriteArrowHover, states);
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
