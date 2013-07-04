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


#ifndef TGUI_WINDOW_HPP
#define TGUI_WINDOW_HPP

#include <queue>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace tgui
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    class TGUI_API Gui : public Container
    {
      public:

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \brief Default constructor
        ///
        /// If you use this constructor then you will still have to call the setWindow yourself.
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        Gui();


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \brief Construct the gui and set the window on which the gui should be drawn.
        ///
        /// \param window  The sfml window that will be used by the gui.
        ///
        /// If you use this constructor then you will no longer have to call setWindow yourself.
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        Gui(sf::RenderWindow& window);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \brief Set the window on which the gui should be drawn.
        ///
        /// \param window  The sfml window that will be used by the gui.
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void setWindow(sf::RenderWindow& window);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \brief Returns the window on which the gui is being drawn.
        ///
        /// \return The sfml that is used by the gui.
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        sf::RenderWindow* getWindow();


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \brief Passes the event to the widgets.
        ///
        /// \param event  The event that was polled from the gui
        ///
        /// You should call this function in your event loop.
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void handleEvent(sf::Event event);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \brief Draws all the widgets that were added to the gui.
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void draw();


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \brief Get the next callback from the callback queue.
        ///
        /// This function works just like the pollEvent function from sfml.
        ///
        /// The gui stores the callback of all the widgets.
        /// This function will return the next callback and then remove it from the queue.
        ///
        /// Note that more than one callbacks may be present in the queue, thus you should always call this
        /// function in a loop to make sure that you process every callback.
        ///
        /// \param callback  An empty tgui::Callback widget that will be (partly) filled when there is a callback.
        ///
        /// \return
        ///        - true when there is another callback. The \a callback parameter will be filled with information.
        ///        - false when there is no callback. The \a callback parameter remains uninitialized and may not be used.
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        bool pollCallback(Callback& callback);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Update the internal clock to make animation possible. This function is called automatically by the draw function.
        // You will thus only need to call it yourself when you are drawing everything manually.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void updateTime(const sf::Time& elapsedTime);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // TGUI uses this function internally to handle widget callbacks.
        // When you tell an widget to send its callbacks to its parent then this function is called.
        //
        // When one or more global callback functions were set then these functions will be called.
        // Otherwise, the callback will be added to the callback queue and you will be able to poll it later with pollCallback.
        //
        // You can use this function to fake an widget callback.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void addChildCallback(Callback& callback);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Used internally to get the size of the window.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        Vector2f getDisplaySize();


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      protected:

        // This will store all widget callbacks until you pop them with getCallback
        std::queue<Callback> m_Callback;

        // The internal clock which is used for animation of widgets
        sf::Clock m_Clock;

        // The sfml window
        sf::RenderWindow* m_Window;

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif // TGUI_WINDOW_HPP
