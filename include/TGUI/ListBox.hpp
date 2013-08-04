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


#ifndef TGUI_LIST_BOX_HPP
#define TGUI_LIST_BOX_HPP

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace tgui
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    class TGUI_API ListBox : public Widget, public WidgetBorders
    {
      public:

        typedef SharedWidgetPtr<ListBox> Ptr;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \brief Default constructor
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ListBox();


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \brief Copy constructor
        ///
        /// \param copy  Instance to copy
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ListBox(const ListBox& copy);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \brief Destructor
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual ~ListBox();


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \brief Overload of assignment operator
        ///
        /// \param right  Instance to assign
        ///
        /// \return Reference to itself
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ListBox& operator= (const ListBox& right);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Makes a copy of the widget by calling the copy constructor.
        // This function calls new and if you use this function then you are responsible for calling delete.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual ListBox* clone();


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \brief Loads the widget.
        ///
        /// \param configFileFilename  Filename of the config file.
        ///
        /// The config file must contain a ListBox section with the needed information.
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


        //////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \brief Changes the size of the list box.
        ///
        /// \param width   The new width of the list box
        /// \param height  The new height of the list box
        ///
        //////////////////////////////////////////////////////////////////////////////////////////////////////
        void setSize(float width, float height);


        //////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \brief Returns the size of the list box
        ///
        /// \return Size of the list box
        ///
        //////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual Vector2f getSize() const;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \brief Changes the colors used in the list box.
        ///
        /// \param backgroundColor          The color of the background of the list box
        /// \param textColor                The color of the text
        /// \param selectedBackgroundColor  The color of the background of the selected item
        /// \param selectedTextColor        The color of the text when it is selected
        /// \param borderColor              The color of the borders
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void changeColors(const sf::Color& backgroundColor         = sf::Color::White,
                          const sf::Color& textColor               = sf::Color::Black,
                          const sf::Color& selectedBackgroundColor = sf::Color(50, 100, 200),
                          const sf::Color& selectedTextColor       = sf::Color::White,
                          const sf::Color& borderColor             = sf::Color::Black);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \brief Set the background color that will be used inside the list box.
        ///
        /// \param backgroundColor  The color of the background of the list box
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void setBackgroundColor(const sf::Color& backgroundColor);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \brief Set the text color that will be used inside the list box.
        ///
        /// \param textColor  The color of the text
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void setTextColor(const sf::Color& textColor);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \brief Set the background color of the selected text that will be used inside the list box.
        ///
        /// \param selectedBackgroundColor  The color of the background of the selected item
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void setSelectedBackgroundColor(const sf::Color& selectedBackgroundColor);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \brief Set the text color of the selected text that will be used inside the list box.
        ///
        /// \param selectedTextColor  The color of the text when it is selected
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void setSelectedTextColor(const sf::Color& selectedTextColor);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \brief Set the border color text that will be used inside the list box.
        ///
        /// \param borderColor  The color of the borders
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void setBorderColor(const sf::Color& borderColor);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \brief Get the background color that is currently being used inside the list box.
        ///
        /// \return The color of the background of the list box
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        const sf::Color& getBackgroundColor() const;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \brief Get the text color that is currently being used inside the list box.
        ///
        /// \return The color of the text
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        const sf::Color& getTextColor() const;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \brief Get the background color of the selected text that is currently being used inside the list box.
        ///
        /// \return The color of the background of the selected item
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        const sf::Color& getSelectedBackgroundColor() const;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \brief Get the text color of the selected text that is currently being used inside the list box.
        ///
        /// \return The color of the text when it is selected
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        const sf::Color& getSelectedTextColor() const;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \brief Get the border color that is currently being used inside the list box.
        ///
        /// \return The color of the borders
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        const sf::Color& getBorderColor() const;


        //////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \brief Changes the font of the items.
        ///
        /// When you don't call this function then the global font will be use.
        /// This global font can be changed with the setGlobalFont function from the parent.
        ///
        /// \param font  The new font.
        ///
        //////////////////////////////////////////////////////////////////////////////////////////////////////
        void setTextFont(const sf::Font& font);


        //////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \brief Returns the font of the items.
        ///
        /// \return  Pointer to the font that is currently being used.
        ///
        //////////////////////////////////////////////////////////////////////////////////////////////////////
        const sf::Font* getTextFont() const;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \brief Adds an item to the list.
        ///
        /// \param itemName  The name of the item you want to add (this is the text that will be displayed inside the list box)
        ///
        /// \return
        ///         -  The index of the item when it was successfully added.
        ///         -  -1 when the list box is full (you have set a maximum item limit and you are trying to add more items)
        ///         -  -1 when there is no scrollbar and you try to have more items than fit inside the list box
        ///
        /// \warning The index returned by this function is no longer correct when an item is removed.
        ///
        /// \see setMaximumItems
        /// \see setScrollbar
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        int addItem(const sf::String& itemName);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \brief Selects an item in the list box.
        ///
        /// When adding items to the list box with the addItem function, none of them will be selected.
        /// If you want a default item selected then you can use this function to select it.
        /// The first item that matches the name will be selected.
        ///
        /// \param itemName  The item you want to select
        ///
        /// \return
        ///         - true on success
        ///         - false when none of the items matches the name
        ///
        /// \see setSelectedItem(unsigned int)
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        bool setSelectedItem(const sf::String& itemName);

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \brief Selects an item in the list box.
        ///
        /// When adding items to the list box with the addItem function, none of them will be selected.
        /// If you want a default item selected then you can use this function to select it.
        ///
        /// \param index  The index of the item
        ///
        /// \return
        ///         - true on success
        ///         - false when the index was too high
        ///
        /// \see setSelectedItem(sf::String)
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        bool setSelectedItem(unsigned int index);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \brief Deselects the selected item.
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void deselectItem();


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \brief Removes an item from the list with a given index.
        ///
        /// \param index  The index of the item to remove
        ///
        /// \return
        ///        - true when the item was removed
        ///        - false when the index was too high
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        bool removeItem(unsigned int index);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \brief Removes the first item from the list with the given name.
        ///
        /// \param itemName  The item to remove
        ///
        /// \return
        ///        - true when the item was removed
        ///        - false when the name didn't match any item
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        bool removeItem(const sf::String& itemName);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \brief Removes all items from the list.
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void removeAllItems();


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \brief Returns the item name of the item with the given index.
        ///
        /// \param index  The index of the item
        ///
        /// \return The requested item.
        ///         The string will be empty when the index was too high.
        ///
        /// \see getItemIndex
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        sf::String getItem(unsigned int index) const;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \brief Returns the index of the first item with the given name.
        ///
        /// \param itemName  The name of the item
        ///
        /// \return The index of the item that matches the name.
        ///         If none of the items matches then the returned index will be -1.
        ///
        /// \warning This index may become wrong when an item is removed from the list.
        ///
        /// \see getItem
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        int getItemIndex(const sf::String& itemName) const;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \brief Returns the list of all the items.
        ///
        /// \return The vector of strings
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        std::vector<sf::String>& getItems();


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \brief Returns the currently selected item.
        ///
        /// \return The selected item.
        ///         When no item was selected then this function will return an empty string.
        ///
        /// \see getSelectedItemIndex
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        sf::String getSelectedItem() const;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \brief Get the index of the selected item.
        ///
        /// \return The index of the selected item
        ///         When no item was selected then this function returns -1.
        ///
        /// \warning This index may become wrong when an item is removed from the list.
        ///
        /// \see getSelectedItem
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        int getSelectedItemIndex() const;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \brief Changes the scrollbar of the list box.
        ///
        /// \param scrollbarConfigFileFilename  Filename of the config file.
        ///                                     The config file must contain a Scrollbar section with the needed information.
        ///
        /// \return
        ///        - true when the scrollbar was successfully loaded
        ///        - false when the loading of the scrollbar failed
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        bool setScrollbar(const std::string& scrollbarConfigFileFilename);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \brief Removes the scrollbar from the list box.
        ///
        /// When there are too many items to fit in the list box then the items will be removed.
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void removeScrollbar();


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \brief Changes the height of the items in the list box.
        ///
        /// \param itemHeight  The size of a single item in the list
        ///
        /// \remarks
        ///         - This size is always a little big greater than the text size.
        ///         - When there is no scrollbar then the items will be removed when they no longer fit inside the list box.
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void setItemHeight(unsigned int itemHeight);

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \brief Returns the height of the items in the list box.
        ///
        /// \return The item height
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        unsigned int getItemHeight() const;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \brief Changes the maximum items that the list box can contain.
        ///
        /// \param maximumItems  The maximum items inside the list box.
        ///                      When the maximum is set to 0 then the limit will be disabled.
        ///
        /// If no scrollbar was loaded then there is always a limitation because there will be a limited space for the items.
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void setMaximumItems(unsigned int maximumItems = 0);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \brief Returns the maximum items that the list box can contain.
        ///
        /// \return The maximum items inside the list box.
        ///         If the function returns 0 then there is no limit.
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        unsigned int getMaximumItems() const;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \brief Changes the size of the borders.
        ///
        /// \param leftBorder    The width of the left border
        /// \param topBorder     The height of the top border
        /// \param rightBorder   The width of the right border
        /// \param bottomBorder  The height of the bottom border
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual void setBorders(unsigned int leftBorder   = 0,
                                unsigned int topBorder    = 0,
                                unsigned int rightBorder  = 0,
                                unsigned int bottomBorder = 0);


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
        virtual void leftMousePressed(float x, float y);
        virtual void leftMouseReleased(float x, float y);
        virtual void mouseMoved(float x, float y);
        virtual void mouseWheelMoved(int delta, int x, int y);
        virtual void mouseNotOnWidget();
        virtual void mouseNoLongerDown();


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
        /// Defines specific triggers to ListBox.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        enum ListBoxCallbacks
        {
            ItemSelected = WidgetCallbacksCount * 1,            ///< A new item was selected
            AllListBoxCallbacks = WidgetCallbacksCount * 2 - 1, ///< All triggers defined in ListBox and its base classes
            ListBoxCallbacksCount = WidgetCallbacksCount * 2
        };


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      protected:

        std::string m_LoadedConfigFile;

        // This contains the different items in the list box
        std::vector<sf::String> m_Items;

        // What is the index of the selected item?
        int m_SelectedItem;

        // The size must be stored
        Vector2u m_Size;
        unsigned int m_ItemHeight;
        unsigned int m_TextSize;

        // This will store the maximum number of items in the list box (zero by default, meaning that there is no limit)
        unsigned int m_MaxItems;

        // When there are too many items a scrollbar will be shown
        Scrollbar* m_Scroll;

        // These colors are used to draw the list box
        sf::Color m_BackgroundColor;
        sf::Color m_TextColor;
        sf::Color m_SelectedBackgroundColor;
        sf::Color m_SelectedTextColor;
        sf::Color m_BorderColor;

        // The font used to draw the text
        const sf::Font* m_TextFont;

        // ComboBox contains a list box internally and it should be able to adjust it.
        friend class ComboBox;

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif // TGUI_LIST_BOX_HPP
