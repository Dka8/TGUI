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


#ifndef TGUI_CHECKBOX_HPP
#define TGUI_CHECKBOX_HPP

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace tgui
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    class TGUI_API Checkbox : public ClickableWidget
    {
      public:

        typedef SharedWidgetPtr<Checkbox> Ptr;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \brief Default constructor
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        Checkbox();


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \brief Copy constructor
        ///
        /// \param copy  Instance to copy
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        Checkbox(const Checkbox& copy);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \brief Destructor
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual ~Checkbox();


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Makes a copy of the widget by calling the copy constructor.
        // This function calls new and if you use this function then you are responsible for calling delete.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual Checkbox* clone();


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \brief Overload of assignment operator
        ///
        /// \param right  Instance to assign
        ///
        /// \return Reference to itself
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        Checkbox& operator= (const Checkbox& right);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \brief Loads the widget.
        ///
        /// \param configFileFilename  Filename of the config file.
        ///
        /// The config file must contain a Checkbox section with the needed information.
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        bool load(const std::string& configFileFilename);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \brief Returns the filename of the config file that was used to load the widget.
        ///
        /// \return Filename of loaded config file.
        ///         Empty string when no config file was loaded yet.
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        const std::string& getLoadedConfigFile();


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \brief Set the position of the widget
        ///
        /// This function completely overwrites the previous position.
        /// See the move function to apply an offset based on the previous position instead.
        /// The default position of a transformable widget is (0, 0).
        ///
        /// \param x X coordinate of the new position
        /// \param y Y coordinate of the new position
        ///
        /// \see move, getPosition
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual void setPosition(float x, float y);
        using Transformable::setPosition;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \brief Changes the size of the checkbox.
        ///
        /// \param width   The new width of the checkbox
        /// \param height  The new height of the checkbox
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual void setSize(float width, float height);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \brief Returns the size of the checkbox.
        ///
        /// \return Size of the checkbox
        ///
        /// The returned size includes the size of the text next to the checkbox.
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual Vector2f getSize() const;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \brief Checks the checkbox.
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual void check();


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \brief Unchecks the checkbox.
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual void uncheck();


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \brief Returns whether the checkbox is checked or not.
        ///
        /// \return Is the checkbox checked?
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        bool isChecked() const;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \brief Changes the text of the checkbox.
        ///
        /// \param text  The new text to draw next to the checkbox.
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void setText(const sf::String& text);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \brief Returns the text of the checkbox.
        ///
        /// \return The text that is drawn next to the checkbox.
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        sf::String getText() const;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \brief Changes the font of the text.
        ///
        /// When you don't call this function then the global font will be use.
        /// This global font can be changed with the setGlobalFont function from the parent.
        ///
        /// \param font  The new font
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void setTextFont(const sf::Font& font);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \brief Returns the font of the text.
        ///
        /// \return Pointer to the font that is currently being used.
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        const sf::Font* getTextFont() const;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \brief Changes the color of the text.
        ///
        /// \param color  New text color
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void setTextColor(const sf::Color& color);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \brief Returns the color of the text.
        ///
        /// \return The current text color
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        const sf::Color& getTextColor() const;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \brief Changes the character size of the text.
        ///
        /// \param size  The new text size.
        ///              When the size is set to 0 then the text is auto-sized.
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void setTextSize(unsigned int size);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \brief Returns the character size of the text.
        ///
        /// \return Character size of the text
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        unsigned int getTextSize() const;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \brief Allow (or disallow) the checkbox to be checked/unchecked by clicking on the text next to the checkbox.
        ///
        /// \param acceptTextClick  Will clicking on the text trigger a checked/unchecked event?
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void allowTextClick(bool acceptTextClick = true);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \brief Changes the transparency of the widget.
        ///
        /// \param transparency  The transparency of the widget.
        ///                      0 is completely transparent, while 255 (default) means fully opaque.
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual void setTransparency(unsigned char transparency);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Used to communicate with EventManager.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual bool mouseOnWidget(float x, float y);
        virtual void leftMouseReleased(float x, float y);
        virtual void keyPressed(sf::Keyboard::Key key);
        virtual void widgetFocused();


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // These functions are a (slow) way to set properties on the widget, no matter what type it is.
        // You can e.g. change the "Text" property, without even knowing that the widget is a button.
        // When the requested property doesn't exist in the widget then the functions will return false.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual bool setProperty(const std::string& property, const std::string& value);
        virtual bool getProperty(const std::string& property, std::string& value);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      protected:

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // This function is called when the widget is added to a container.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual void initialize(Container *const container);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Draws the widget on the render target.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      public:

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// Defines specific triggers to Checkbox.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        enum CheckboxCallbacks
        {
            Checked = ClickableWidgetCallbacksCount * 1,                   ///< Checkbox was checked
            Unchecked = ClickableWidgetCallbacksCount * 2,                 ///< Checkbox was unchecked
            SpaceKeyPressed = ClickableWidgetCallbacksCount * 8,           ///< Space key was pressed
            ReturnKeyPressed = ClickableWidgetCallbacksCount * 16,         ///< Return key was pressed
            AllCheckboxCallbacks = ClickableWidgetCallbacksCount * 32 - 1, ///< All trigger defined in Checkbox and its base classes
            CheckboxCallbacksCount = ClickableWidgetCallbacksCount * 32
        };


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      protected:

        std::string m_LoadedConfigFile;

        // This is the checked flag. When the checkbox is checked then this variable will be true.
        bool m_Checked;

        // When this boolean is true (default) then the checkbox will also be checked/unchecked by clicking on the text.
        bool m_AllowTextClick;

        // This will contain the text that is written next to checkbox.
        sf::Text m_Text;

        // This will store the size of the text ( 0 to auto size )
        unsigned int m_TextSize;

        Texture m_TextureUnchecked;
        Texture m_TextureChecked;
        Texture m_TextureHover;
        Texture m_TextureFocused;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif // TGUI_CHECKBOX_HPP
