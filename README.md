# SA-MP/Open.MP Server Project

A modern SA-MP/Open.MP server implementation with advanced features including CEF-based UI, skin customization system, and authentication.

## 📋 Table of Contents

- [Features](#features)
- [Project Structure](#project-structure)
- [Installation](#installation)
- [Configuration](#configuration)
- [Code Style Guidelines](#code-style-guidelines)
- [Modules](#modules)
- [API Documentation](#api-documentation)
- [Contributing](#contributing)
- [License](#license)

## ✨ Features

### Core Features
- **Modern Authentication System** - Secure player authentication with launcher token verification
- **CEF Integration** - Chromium Embedded Framework for modern web-based UI
- **Advanced Skin System** - Comprehensive character customization with attachments
- **Database Integration** - MySQL database for persistent data storage
- **Modular Architecture** - Organized codebase with separate modules

### Skin Customization System
- **Multiple Attachment Types**: Hats, Glasses, Masks, Backpacks
- **Real-time Editing**: Live preview and positioning of attachments
- **Outfit Management**: Save, load, and manage multiple outfits
- **Database Persistence**: All customizations saved to database

### UI Features
- **Web-based Interface**: Modern HTML/CSS/JS frontend
- **Responsive Design**: Works on different screen resolutions
- **Real-time Updates**: Live synchronization between UI and game

## 📁 Project Structure

```
Server/
├── gamemodes/
│   ├── main.inc              # Main gamemode include file
│   ├── main.amx              # Compiled gamemode
│   ├── main.xml              # Gamemode metadata
│   └── modules/
│       ├── include.inc       # Module includes
│       ├── auth/             # Authentication module
│       ├── skin/             # Skin customization module
│       └── utils/            # Utility functions
├── plugins/                  # Server plugins
├── components/               # Additional components
├── scriptfiles/             # Server script files
├── models/                  # Custom models
├── filterscripts/           # Filter scripts
├── logs/                    # Server logs
├── config.json              # Server configuration
└── README.md               # This file
```

## 🚀 Installation

### Prerequisites
- SA-MP Server or Open.MP Server
- MySQL Database
- CEF Plugin
- Required Plugins (see below)

### Required Plugins
```ini
plugins = PawnPlus,omp_cmd,a_mysql,sscanf2,samp_bcrypt,cef,easyDialog
```

### Database Setup
1. Create a MySQL database
2. Import the required tables (see `database/` folder)
3. Update database connection settings in configuration

### Server Configuration
1. Copy all files to your server directory
2. Update `config.json` with your settings
3. Ensure all plugins are in the `plugins/` directory
4. Start the server

## ⚙️ Configuration

### Main Configuration (`config.json`)
```json
{
  "server": {
    "max_players": 100,
    "port": 7777,
    "hostname": "Your Server Name"
  },
  "database": {
    "host": "localhost",
    "user": "username",
    "password": "password",
    "database": "samp_server"
  },
  "cef": {
    "browser_id": "0xABCDE",
    "interface_url": "http://localhost:3000"
  }
}
```

### CEF Configuration
- **Browser ID**: `0xABCDE` (defined in main.inc)
- **Interface URL**: `http://localhost:3000` (web interface location)

## 📝 Code Style Guidelines

### General Principles
- **Consistent Indentation**: Use 4 spaces for indentation
- **Clear Comments**: Add descriptive comments for complex logic
- **Organized Structure**: Group related functions together
- **Meaningful Names**: Use descriptive variable and function names

### Include Organization
```pawn
// Core includes
#include <open.mp>
#include <PawnPlus>

// Configuration
#define MAX_PLAYERS 100

// Additional includes
#include <sscanf2>
#include <cef>

// Module includes
#include "modules/include"
```

### Function Documentation
```pawn
// =============================================================================
// Function Name
// =============================================================================
// Description: What the function does
// Parameters: playerid - Player ID
// Returns: 1 on success, 0 on failure
public FunctionName(playerid)
{
    // Function implementation
    return 1;
}
```

### Event Organization
```pawn
// =============================================================================
// Player Events
// =============================================================================

public OnPlayerConnect(playerid)
{
    // Implementation
    return 1;
}

public OnPlayerDisconnect(playerid, reason)
{
    // Implementation
    return 1;
}
```

### Switch Statements
```pawn
switch (variable)
{
    case 1: // Case description
    {
        // Implementation
    }
    case 2: // Case description
    {
        // Implementation
    }
    default:
    {
        // Default case
    }
}
```

## 🔧 Modules

### Authentication Module (`modules/auth/`)
- Player authentication system
- Launcher token verification
- Session management

### Skin Module (`modules/skin/`)
- Character customization
- Attachment management
- Outfit system

### Utils Module (`modules/utils/`)
- Utility functions
- Helper macros
- Common operations

## 📚 API Documentation

### CEF Events
```pawn
// Subscribe to UI events
cef_subscribe("ui:createCharacter", "authOnCreateCharacter");
cef_subscribe("ui:selectSkin", "skinOnSelectSkin");

// Create browser instance
cef_create_browser(playerid, INTERFACE_BROWSER_ID, INTERFACE_BROWSER_URL, false, false);
```

### Database Operations
```pawn
// Format and execute queries
mysql_format(g_SQL, query, sizeof(query), "SELECT * FROM table WHERE id = %d", playerid);
mysql_query(g_SQL, query);
```

### Skin System
```pawn
// Set player attachment
SetPlayerAttachedObject(playerid, slot, modelid, bone, 
    offsetX, offsetY, offsetZ, 
    rotX, rotY, rotZ, 
    scaleX, scaleY, scaleZ);
```

## 🤝 Contributing

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

### Code Review Guidelines
- Follow the established code style
- Add appropriate comments
- Test your changes thoroughly
- Update documentation if needed

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 🆘 Support

If you encounter any issues or have questions:
1. Check the [Issues](https://github.com/your-repo/issues) page
2. Review the documentation
3. Contact the development team

---

**Note**: This is a work in progress. Features and documentation may be updated regularly. 