# 🎵 C-Unplugged CLI

> A modular, command-line music management system written in C — manage songs, albums, and playlists powered by dynamic linked lists and file-based persistence.

---

## 📋 Table of Contents

- [Overview](#overview)
- [Features](#features)
- [Data Structures](#data-structures)
- [Project Structure](#project-structure)
- [Getting Started](#getting-started)
- [Usage](#usage)
- [File Persistence](#file-persistence)
- [Technical Notes](#technical-notes)

---

## Overview

C-Unplugged is a terminal-based music player simulator built entirely in C. It loads your music library at startup, lets you manage songs, albums, and playlists through an interactive menu, and saves your data before exiting. Designed as a showcase of core data structure concepts — linked lists, circular doubly linked lists, and nested list architectures — applied to a real-world use case.

---

## Features

### 🎼 Song Library
- Load songs from `songs.txt` at startup
- Add new songs to the library at runtime
- Display all songs with dynamic indexing
- Implemented as a **singly linked list**

### 💿 Album Management
- Create and delete albums
- View album contents
- Add or remove songs from an album
- Songs are stored as **independent copies** inside albums (data isolation model)
- Albums persist across sessions via `albums.txt`
- Implemented as a **nested linked list** (list of albums, each containing a list of songs)

### ▶️ Playlist System
- Add individual songs or entire albums to the playlist
- View the current playlist
- Play current, next, or previous song — all in **O(1)** time
- Remove songs from the playlist
- **Infinite loop playback** — wraps seamlessly from last to first song
- Implemented as a **Circular Doubly Linked List**
- Runtime-only (not persisted between sessions)

### 📝 Command History
- Every executed command is automatically logged
- Stored in `commands.log`
- Append-only — persists across all program sessions
- No in-memory overhead after exit

---

## Data Structures

| Component | Structure | Details |
|-----------|-----------|---------|
| Song Library | Singly Linked List | Dynamic indexing; global head pointer |
| Albums | Nested Singly Linked Lists | Outer list of albums, each with its own inner song list |
| Playlist | Circular Doubly Linked List | Head, tail, and current-play pointers; O(1) prev/next |
| Command History | File-based Log | Append-only; not held in memory |

### Why These Choices?

**Singly Linked List (Song Library & Albums):** Simple, memory-efficient, and well-suited for sequential access and iteration — ideal for a library you browse top to bottom.

**Nested Linked List (Albums):** Albums are a natural hierarchy. An outer list of album nodes, each owning an inner list of song copies, keeps data neatly isolated — changes to a song in an album don't affect the master library.

**Circular Doubly Linked List (Playlist):** Enables true O(1) forward and backward navigation and seamless infinite looping without bounds-checking — perfect for continuous playback.

---

## Project Structure

```
c-unplugged/
│
├── src/
│   ├── main.c              # Entry point and menu loop
│   ├── songs.c / songs.h   # Song library logic
│   ├── albums.c / albums.h # Album management logic
│   └── playlist.c / .h     # Playlist (CDLL) logic
│
├── data/
│   ├── songs.txt           # Persistent song library
│   └── albums.txt          # Persistent album data
│
├── logs/
│   └── commands.log        # Append-only command history
│
└── Makefile
```

---

## Getting Started

### Prerequisites

- GCC or any C99-compatible compiler
- Make (optional but recommended)

### Build

```bash
git clone https://github.com/your-username/c-unplugged.git
cd c-unplugged
make
```

Or compile manually:

```bash
gcc -o c-unplugged src/main.c src/songs.c src/albums.c src/playlist.c
```

### Run

```bash
./c-unplugged
```

The program will automatically load `songs.txt` and `albums.txt` on startup.

---

## Usage

Upon launching, you'll be presented with an interactive menu:

```
=============================
     🎵 C-Unplugged CLI
=============================
 [1] Song Library
 [2] Album Management
 [3] Playlist
 [4] View Command History
 [0] Exit
=============================
```

Navigate submenus to manage your songs, albums, and playlist. All commands are logged automatically to `commands.log`.

---

## File Persistence

| File | Contents | Mode |
|------|----------|------|
| `songs.txt` | Song library (title, artist, duration) | Read on startup, write on exit |
| `albums.txt` | Album names and their song lists | Read on startup, write on exit |
| `commands.log` | All executed commands with timestamps | Append-only |

> **Note:** The playlist is session-only and is not saved between runs.

---

## Technical Notes

- Songs stored inside albums are **deep copies**, ensuring full data isolation from the master library.
- The Circular Doubly Linked List maintains a `current` pointer that persists between play operations, enabling seamless "resume" behavior.
- Command logging uses `fprintf` in append mode (`"a"`), making it safe for incremental logging without risk of data loss.
- Memory is fully freed on exit to prevent leaks.

---

