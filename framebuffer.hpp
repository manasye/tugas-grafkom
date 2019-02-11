#ifndef FBUFFER_HPP
#define FBUFFER_HPP

#include <stdint.h>
#include <thread>

class Framebuffer {
    public:
        // Framebuffer ctor
        // Initialize the framebuffer : gets info, a shared-mapping to the framebuffer, and a back-buffer
        Framebuffer();
        // Framebuffer dtor
        // Removes the shared-mapping and deallocates back-buffer
        ~Framebuffer();
        // Getter
        int getXRes();
        int getYRes();
        // Flush the framebuffer with black
        void clear();
        // Flush current buffer to frame buffer and swaps the current buffer
        void swapBuffer();
        // Write a pixel in the current buffer
        // If x or y is out-of-bounds, this function will do nothing
        // To apply changes to screen, use swapBuffer()
        void setPixel(short x, short y, uint32_t rgb);

    private:
        uint32_t * frameBuffer;
        uint32_t * backBuffer1;
        uint32_t * backBuffer2;
        uint32_t * currentBuffer;
        int xres;
        int yres;
        std::thread * flushThread;
        int firstTime;

        void flush();
};

#include "framebuffer.cpp"

#endif