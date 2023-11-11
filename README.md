# RDR2 Native Menu Base
An RDR2 UI Menu Base that uses in-game sprites to make UI menus similar to how they are in-game

# Sample Images
- These examples are included in the source code, and are easy to remove if needed
<details>
  <summary>Basic Samples (Outdated) (Taken on June 15th, 2022)</summary>
  <p>
    <img src="https://i.imgur.com/G8fB6r2.png" alt="Regular Option" height="600">
    <img src="https://i.imgur.com/ag6g1sG.png" alt="Bool Option" height="600">
    <img src="https://i.imgur.com/IEH5QkX.png" alt="Vector Option" height="600">
	<img src="https://i.imgur.com/psHtOkd.png" alt="Vector Option With Changed Value" height="600">
	<img src="https://i.imgur.com/w8A45pW.png" alt="Submenu Example" height="600">
	<img src="https://i.imgur.com/vNgMQ5x.png" alt="Another submenu" height="600">
  </p>
</details>

# Abstraction Branch
- The abstraction branch's purpose is to implement more abstraction/polymorphism into this menu base
- It has its ups and downs, but I implemented this because I thought some people might prefer this over the master branch

# Features
- Easy to configure, organize, and use
- Keyboard and controller support with rebindable keybinds
- Faster scrolling keybind (up to 10 options at a time)
- Ability to draw your own custom text on the screen with fonts that Rockstar uses
- Debug console for testing purposes
- And more

# Option Types
- Submenu Options
	- These options will bring up another page when clicked that can bring up more options to choose from.
- Regular Options
	- These options will immediately execute your custom code when clicked.
- Bool Options
	- These options are like an on/off switch. When clicked, the passed boolean pointer will be switched from true to false and vice versa, allowing you to execute your custom code.
- Vector Options
	- These options contain multiple selections of choosable items. If you know what an std::vector is, then this option is basically that. Everytime this option is switched, your custom code will execute.
- Empty Options
	- These options will insert a blank spot into the page. Mainly used for organization purposes.

# Help & Info
- script.cpp contains the main loop
- console.h contains useful macros that print things to the included debug console
- The "Submenus" folder is where you can create your submenus
- This folder also contains examples of how you might set submenus up.
- To change keybinds, see UI/Menu.cpp in CNativeMenu::CheckInput()
- UI/Options/ contains all option types
- UI/Menu.hpp is the CNativeMenu singleton class, which contains g_Menu global
- UI/Menu.cpp handles keypresses, navigation, some drawing, and other things
- UI/Submenu.h is the base CSubmenu class, meant to be inherited from by your custom submenus. See Submenus/Examples.h and Submenus/EntryMenu.h
- UI/Drawing.cpp draws all text, textures, options, etc
- Note: EntryMenu.h/.cpp is the very first submenu

# Changelog
- See: [changelog.txt](https://github.com/Halen84/RDR2-Native-Menu-Base/blob/abstraction/changelog.txt)

# TODO List + Bugs
- More support for displays that arent 1920x1080 (mostly done)
- Better/Improved scrolling
- Mouse support

# Credits
- Alexander Blade for the Scripthook SDK for RDR2
- GitHub contributors
