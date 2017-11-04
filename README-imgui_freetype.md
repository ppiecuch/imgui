## dear imgui meet FreeType,

This is an attempt to replace stb_truetype (the default imgui's font rasterizer) with FreeType.

Probably lots of bugs, not production ready.
Only the dx11 example is working and only when using Visual Studio 2015.
But it's not too hard to adapt it in your code:

1. Get latest FreeType binaries or build yourself.
2. Add imgui_freetype.h/cpp alongside your imgui sources.
3. Include imgui_freetype.h after imgui.h.
4. Call ImGuiFreeType::BuildFontAtlas() prior to calling GetTexDataXXX (and normal Build() won't be called):

```cpp
// see ImGuiFreeType::RasterizationFlags
unsigned int flags = ImGuiFreeType::DisableHinting;
ImGuiFreeType::BuildFontAtlas(io.Fonts, flags);
io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);
```

### Known issues:
1. Excessive font texture resolution.
2. FreeType's memory allocator is not overriden.


### Obligatory comparison screenshots
Using Windows built-in segoeui.ttf font.
Open in new browser tabs, view at 1080p+.

Built-in stb_truetype rasterizer:
![Vanilla](screenshots/stb.png)

FreeType's (auto) hinting rasterizer:
![FreeType](screenshots/freetype.png)