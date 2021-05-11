/*
 * Copyright Null LLC
 * Please read the License!
 *  _     _           _ 
 * | |   | |         | |
 * | |__ | |_   _  _ | | ____ ____
 * |  __)| | | | |/ || |/ ___) _  |     panic.hpp
 * | |   | | |_| ( (_| | |  ( ( | |     Controls the functions called during a kernel panic event.
 * |_|   |_|\__  |\____|_|   \_||_|
 *         (____/
 */

#pragma once

void Panic(const char* panicMessage);