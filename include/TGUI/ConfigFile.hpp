/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// TGUI - Texus's Graphical User Interface
// Copyright (C) 2012 Bruno Van de Velde (vdv_b@tgui.eu)
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


#ifndef TGUI_CONFIG_FILE_HPP
#define TGUI_CONFIG_FILE_HPP

#include <fstream>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace tgui
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Reads the config files that are used to load objects.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    class TGUI_API ConfigFile : public sf::NonCopyable
    {
      public:

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // \brief Destructor. Closes the file if still open.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ~ConfigFile();


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Open a given file. If a file is already open then it will be closed first.
        //
        // return:  true when the file was opened
        //          false when it couldn't be opened
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        bool open(const std::string& filename);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Reads a specific section from the file and returns the corresponding properties and values.
        //
        // return:  true when a section was found
        //          false when there was no section with the given name
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        bool read(std::string section, std::vector<std::string>& properties, std::vector<std::string>& values);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Reads a bool from a string value.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        bool readBool(const std::string& value, bool defaultValue) const;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Reads a color from a string value.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        sf::Color readColor(const std::string& value) const;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Reads an int rectangle from a string value.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        bool readIntRect(std::string value, sf::IntRect& rect) const;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Loads a texture based on the contents of the string value.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        bool readTexture(const std::string& value, const std::string& rootPath, Texture& texture) const;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Close the file that was opened by the open function.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void close();


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      private:

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Remove the whitespace from the line, starting from the character c.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        bool removeWhitespace(const std::string& line, std::string::const_iterator& c) const;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Reads the next word, starting from character c.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        std::string readWord(const std::string& line, std::string::const_iterator& c) const;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Check if line is a section header.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        bool isSection(const std::string& line, std::string::const_iterator c, std::string& sectionName) const;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      private:

        std::ifstream m_File;
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif // TGUI_CONFIG_FILE_HPP

