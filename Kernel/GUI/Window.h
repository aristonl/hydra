#pragma once

class Window {
    public:
        Window();
        Window(const char* name, unsigned int x, unsigned int y, unsigned int width, unsigned int height, bool min, bool max);
        void destroy();
        unsigned int x, y, width, height;
        const char *name;
};