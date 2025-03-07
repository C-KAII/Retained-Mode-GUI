// Renderer.h
#pragma once
#include "Utils.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <iostream>

class Renderer {
public:
  Renderer() = default;

  ~Renderer() {
    SDL_StopTextInput();
    if (m_font) { TTF_CloseFont(m_font); }
    if (m_renderer) { SDL_DestroyRenderer(m_renderer); }
    if (m_window) { SDL_DestroyWindow(m_window); }
    TTF_Quit();
    SDL_Quit();
  }

  bool init(const std::string& title, int width, int height) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
      std::cerr << "SDL Initialization Failed: " << SDL_GetError() << std::endl;
      return false;
    }

    m_window = SDL_CreateWindow(
      title.c_str(),
      SDL_WINDOWPOS_CENTERED,
      SDL_WINDOWPOS_CENTERED,
      width,
      height,
      SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
    );

    if (!m_window) {
      std::cerr << "Window Creation Failed: " << SDL_GetError() << std::endl;
      return false;
    }

    m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!m_renderer) {
      std::cerr << "Renderer Creation Failed: " << SDL_GetError() << std::endl;
      SDL_DestroyWindow(m_window);
      return false;
    }

    m_screenWidth = width;
    m_screenHeight = height;
    return true;
  }

  bool initFont(const std::string& fontPath, int fontSize ) {
    if (TTF_Init() == -1) {
      std::cerr << "SDL_ttf could not initialize! Error: " << TTF_GetError() << std::endl;
      return false;
    }

    m_font = TTF_OpenFont(fontPath.c_str(), fontSize);
    if (!m_font) {
      std::cerr << "Failed to load font! Error: " << TTF_GetError() << std::endl;
      return false;
    }

    TTF_SizeUTF8(m_font, "A", &m_fontW, &m_fontH);
    SDL_StartTextInput();
    return true;
  }

  void drawRect(int x, int y, int w, int h, SDL_Color color) {
    SDL_Rect rect{ x, y, w, h };
    SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(m_renderer, &rect);
  }

  void drawRect(SDL_Rect rect, SDL_Color color) {
    SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(m_renderer, &rect);
  }

  SDL_Texture* createTextTexture(const std::string& text, SDL_Color color) {
    if (text.empty()) { return nullptr; }

    // Create surface from font
    SDL_Surface* textSurface = TTF_RenderText_Solid(m_font, text.c_str(), color);
    if (!textSurface) {
      std::cerr << "Text rendering failed: " << TTF_GetError() << std::endl;
      return nullptr;
    }

    // Convert surface to texture
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(m_renderer, textSurface);
    SDL_FreeSurface(textSurface); // Free surface after converting
    
    if (!textTexture) {
      std::cerr << "Texture creation failed: " << SDL_GetError() << std::endl;
    }

    return textTexture;
  }

  void drawText(const std::string& text, int x, int y, SDL_Color color = UTILS::COLOR::BLACK) {
    SDL_Texture* textTexture = createTextTexture(text, color);
    if (!textTexture) { return; }

    // Get text dimensions
    int textW = 0;
    int textH = 0;
    TTF_SizeText(m_font, text.c_str(), &textW, &textH);

    SDL_Rect dstRect = { x, y, textW, textH };

    // Render text
    SDL_RenderCopy(m_renderer, textTexture, nullptr, &dstRect);
    SDL_DestroyTexture(textTexture);
  }

  void drawTextCentered(const std::string& text, int x, int y, int w, int h, SDL_Color color = UTILS::COLOR::BLACK) {
    SDL_Texture* textTexture = createTextTexture(text, color);
    if (!textTexture) { return; }

    // Get text dimensions
    int textW = 0;
    int textH = 0;
    TTF_SizeText(m_font, text.c_str(), &textW, &textH);

    int centerX = x + ((w - textW) / 2);
    int centerY = y + ((h - textH) / 2);

    if (centerX < x) { centerX = x; }
    if (centerY < y) { centerY = y; }

    SDL_Rect dstRect = { centerX, centerY, textW, textH };

    // Render text
    SDL_RenderCopy(m_renderer, textTexture, nullptr, &dstRect);
    SDL_DestroyTexture(textTexture);
  }

  void clear(SDL_Color color) {
    SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, 255);
    SDL_RenderClear(m_renderer);
  }

  void present() { SDL_RenderPresent(m_renderer); }

  SDL_Renderer* getRenderer() { return m_renderer; }

  int getFontWidth() const { return m_fontW; }
  int getFontHeight() const { return m_fontH; }

  void updateWindowSize(int width, int height) {
    if (m_renderer) {
      SDL_RenderSetLogicalSize(m_renderer, width, height);
    }

    m_screenWidth = width;
    m_screenHeight = height;
  }

  int getScreenWidth() const { return m_screenWidth; }
  int getScreenHeight() const { return m_screenHeight; }

private:
  SDL_Window* m_window{ nullptr };
  SDL_Renderer* m_renderer{ nullptr };
  TTF_Font* m_font{ nullptr };

  int m_screenWidth{ 0 };
  int m_screenHeight{ 0 };

  int m_fontW{ 0 };
  int m_fontH{ 0 };
};