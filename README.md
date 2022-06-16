# RDR2 Native Menu Base
An RDR2 UI Menu Base that uses in-game sprites to make UI menus similar to how they are in-game

# Sample Images
- These examples are included in the source code, and are easy to remove if needed
<details>
  <summary>Basic Samples (Taken on April 2022)</summary>
  <p>
    <img src="https://i.imgur.com/oAKUmCe.png" alt="Regular Option" height="600">
    <img src="https://i.imgur.com/mUBXHm0.png" alt="Bool Option" height="600">
    <img src="https://i.imgur.com/6m8GGIY.png" alt="Vector Option" height="600">
	<img src="https://i.imgur.com/01YiWZl.png" alt="Vector Option With Changed Value" height="600">
	<img src="https://i.imgur.com/a6YVkJ0.png" alt="Submenu" height="600">
	<img src="https://i.imgur.com/dojQ7G7.png" alt="Another submenu" height="600">
  </p>
</details>

# Features
- Easy to configure
- Easy to add new pages and more options
- Keyboard and controller support with rebindable keybinds
- Faster scrolling keybind (up to 10 options at a time)
- Ability to draw your own custom text on the screen with fonts that Rockstar uses

# Option Types
- Submenu Options
	- These options will bring up another page when clicked that can bring up more options to choose from.
- Regular Options
	- These options will immediately execute your custom code when clicked.
- Bool Options
	- These options are like an on/off switch. When clicked, the passed boolean pointer will be switched from true to false and vice versa, allowing you to execute your custom code.
- Vector Options
	- These options contain multiple selections of choosable items. If you know what an std::vector is, then this option is basically that. Everytime this option is switched, your custom code will execute.

# Help & Info
- script.cpp would be where to look first
- The UI will look and function the best on 1920x1080 displays
- The "SubMenus" folder is where you can put your functions for options. Its up to you how you want to organize everything.
- ^^^ You can also create the options in other files as long as you call it in InitializeMenu()
- UI/option.hpp is the Option class
- UI/menu.hpp is the CNativeMenu class
- UI/submenu.hpp is the Submenu class
- UI/menu.cpp handles keypresses, navigation, some drawing, and other things
- UI/drawing.cpp does the texture and text drawing for everything

# Changelog
- See: [changelog.txt](https://github.com/Halen84/RDR2-Native-Menu-Base/blob/master/changelog.txt)

# Credits
- Alexander Blade for the Scripthook SDK for RDR2
- GitHub contributors
