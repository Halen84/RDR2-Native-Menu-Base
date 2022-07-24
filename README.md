# RDR2 Native Menu Base
An RDR2 UI Menu Base that uses in-game sprites to make UI menus similar to how they are in-game

# Sample Images
- These examples are included in the source code, and are easy to remove if needed
<details>
  <summary>Basic Samples (Taken on June 15th, 2022)</summary>
  <p>
    <img src="https://i.imgur.com/G8fB6r2.png" alt="Regular Option" height="600">
    <img src="https://i.imgur.com/ag6g1sG.png" alt="Bool Option" height="600">
    <img src="https://i.imgur.com/IEH5QkX.png" alt="Vector Option" height="600">
	<img src="https://i.imgur.com/psHtOkd.png" alt="Vector Option With Changed Value" height="600">
	<img src="https://i.imgur.com/w8A45pW.png" alt="Submenu Example" height="600">
	<img src="https://i.imgur.com/vNgMQ5x.png" alt="Another submenu" height="600">
  </p>
</details>

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
- Page Breaks
	- These options will insert a blank spot into the page. Mainly used for organization purposes.

# Help & Info
- script.cpp would be where to look first
- The UI will look and function the best on 1920x1080 displays
- The "SubMenus" folder is where you can put your functions for options. Its up to you how you want to organize everything.
- ^^^ You can also create the options in other files as long as you call it in InitializeMenu()
- To change keybinds, see UI/menu.cpp in CheckInput()
- UI/option.hpp is the Option class
- UI/menu.hpp is the CNativeMenu class
- UI/submenu.hpp is the Submenu class
- UI/menu.cpp handles keypresses, navigation, some drawing, and other things
- UI/drawing.cpp does the texture and text drawing for everything

# Changelog
- See: [changelog.txt](https://github.com/Halen84/RDR2-Native-Menu-Base/blob/master/changelog.txt)

# TODO List + Bugs
- More support for displays that arent 1920x1080
- Better/Improved scrolling to feel more natural
- Fix font sizes on different displays
- Fix certain UI positions on different displays

# Credits
- Alexander Blade for the Scripthook SDK for RDR2
- GitHub contributors
