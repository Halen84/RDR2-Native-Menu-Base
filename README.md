# RDR2 Native Menu Base
An RDR2 UI Menu Base that uses in-game sprites to make similar menus to how they are in-game

# Sample Images
- These examples are included in the source code, and are easy to remove if needed
<details>
  <summary>Basic Samples (Taken on April 18th, 2022)</summary>
  <p>
    <img src="https://i.imgur.com/oAKUmCe.png" alt="Regular Option" height="600">
    <img src="https://i.imgur.com/mUBXHm0.png" alt="Toggle Option" height="600">
    <img src="https://i.imgur.com/6m8GGIY.png" alt="Vector Option" height="600">
	<img src="https://i.imgur.com/01YiWZl.png" alt="Vector Option With Changed Value" height="600">
	<img src="https://i.imgur.com/a6YVkJ0.png" alt="New Page" height="600">
	<img src="https://i.imgur.com/dojQ7G7.png" alt="Another Page" height="600">
  </p>
</details>

# Features
- Easy to configure
- Easy to add new pages and more options
- Keyboard and controller rebindable keybinds
- Faster scrolling keybind (up to 10 options at a time)
- Ability to draw your own custom text on the screen with fonts that Rockstar uses

# Option Types
- Page Options
	- These options will bring up another page when clicked that can bring up more options to choose from.
- Regular Options
	- These options will immediately execute your custom code when clicked.
- Toggle Options
	- These options are like an on/off switch. When clicked, the passed boolean pointer will be switched from true to false and vice versa, allowing you to execute your custom code.
- Vector Options
	- These options contain multiple selections of items. If you know what an std::vector is, then this option is basically that. Everytime this option is switched, your custom code will execute. You will be able to get the current selection a vector is on.

# Help & Info
- To create new pages and options, see pages.cpp
- To execute code when an option is pressed/toggled, see functions.cpp (any pages.cpp)
- menu.cpp is basically the core UI functionality. Edit at your own risk.
- script.cpp (generic .asi main loop file) is also core. Handles key presses, indexing, drawing, and some other things.
- Might be a good idea to "using namespace Menu;" in your files to speed up your coding slightly. Kinda ruins the point of namespaces but I don't care.
- The "lib" folder is useful if you want to import external libraries to use. You can just throw them in there.
- Ignore the "xyz is ambiguous" errors! It's just an intellisense error.

# Changelog
- See: [changelog.txt](https://github.com/Halen84/RDR2-Native-Menu-Base/blob/master/changelog.txt)

# Credits
- Alexander Blade for the Scripthook SDK for RDR2
- GitHub contributors

#

The codebase isn't the best because I'm not a C++ master. However, it's 100% do-able to work it.

I'm always looking to improve this codebase. One day, it will get a complete overhaul and rewrite. It's already gotten like 2 rewrites, but they aren't as big as the overhaul will be.