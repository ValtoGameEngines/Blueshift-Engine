// Copyright(c) 2017 POLYGONTEK
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
// http ://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "Precompiled.h"
#include "Core/Str.h"
#include "Math/Math.h"
#include "Image/Image.h"
#include "ImageInternal.h"

BE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------
//
// XXXToRGBA8888 (unpacking function from custom format to rgba8888)
//
//--------------------------------------------------------------------------------------------------

static void L8ToRGBA8888(const byte *src, byte *dst, int numPixels, bool isGamma) {
    const byte *srcPtr = src;
    const byte *srcEnd = srcPtr + numPixels;
    byte *dstPtr = dst;

    for (; srcPtr < srcEnd; srcPtr += 1, dstPtr += 4) {
        dstPtr[0] = srcPtr[0];
        dstPtr[1] = srcPtr[0];
        dstPtr[2] = srcPtr[0];
        dstPtr[3] = 255;
    }
}
static void A8ToRGBA8888(const byte *src, byte *dst, int numPixels, bool isGamma) {
    const byte *srcPtr = src;
    const byte *srcEnd = srcPtr + numPixels;
    byte *dstPtr = dst;

    for (; srcPtr < srcEnd; srcPtr += 1, dstPtr += 4) {
        dstPtr[0] = 255;
        dstPtr[1] = 255;
        dstPtr[2] = 255;
        dstPtr[3] = srcPtr[0];
    }
}
static void LA88ToRGBA8888(const byte *src, byte *dst, int numPixels, bool isGamma) {
    const byte *srcPtr = src;
    const byte *srcEnd = srcPtr + numPixels * 2;
    byte *dstPtr = dst;

    for (; srcPtr < srcEnd; srcPtr += 2, dstPtr += 4) {
        dstPtr[0] = srcPtr[0];
        dstPtr[1] = srcPtr[0];
        dstPtr[2] = srcPtr[0];
        dstPtr[3] = srcPtr[1];
    }
}
static void LA1616ToRGBA8888(const byte *src, byte *dst, int numPixels, bool isGamma) {
    const uint16_t *srcPtr = (const uint16_t *)src;
    const uint16_t *srcEnd = srcPtr + numPixels * 2;
    byte *dstPtr = dst;
    byte l, a;

    for (; srcPtr < srcEnd; srcPtr += 2, dstPtr += 4) {
        l = srcPtr[0] >> 8;
        a = srcPtr[1] >> 8;
        dstPtr[0] = l;
        dstPtr[1] = l;
        dstPtr[2] = l;
        dstPtr[3] = a;
    }
}
static void R8ToRGBA8888(const byte *src, byte *dst, int numPixels, bool isGamma) {
    const byte *srcPtr = src;
    const byte *srcEnd = srcPtr + numPixels;
    byte *dstPtr = dst;

    for (; srcPtr < srcEnd; srcPtr += 1, dstPtr += 4) {
        dstPtr[0] = srcPtr[0];
        dstPtr[1] = 0;
        dstPtr[2] = 0;
        dstPtr[3] = 255;
    }
}
static void RG88ToRGBA8888(const byte *src, byte *dst, int numPixels, bool isGamma) {
    const byte *srcPtr = src;
    const byte *srcEnd = srcPtr + numPixels * 2;
    byte *dstPtr = dst;

    for (; srcPtr < srcEnd; srcPtr += 2, dstPtr += 4) {
        dstPtr[0] = srcPtr[0];
        dstPtr[1] = srcPtr[1];
        dstPtr[2] = 0;
        dstPtr[3] = 255;
    }
}
static void RGB888ToRGBA8888(const byte *src, byte *dst, int numPixels, bool isGamma) {
    const byte *srcPtr = src;
    const byte *srcEnd = srcPtr + numPixels * 3;
    byte *dstPtr = dst;

    for (; srcPtr < srcEnd; srcPtr += 3, dstPtr += 4) {
        dstPtr[0] = srcPtr[0];
        dstPtr[1] = srcPtr[1];
        dstPtr[2] = srcPtr[2];
        dstPtr[3] = 255;
    }
}
static void BGR888ToRGBA8888(const byte *src, byte *dst, int numPixels, bool isGamma) {
    const byte *srcPtr = src;
    const byte *srcEnd = srcPtr + numPixels * 3;
    byte *dstPtr = dst;

    for (; srcPtr < srcEnd; srcPtr += 3, dstPtr += 4) {
        dstPtr[0] = srcPtr[2];
        dstPtr[1] = srcPtr[1];
        dstPtr[2] = srcPtr[0];
        dstPtr[3] = 255;
    }
}
static void RGBX8888ToRGBA8888(const byte *src, byte *dst, int numPixels, bool isGamma) {
    const byte *srcPtr = src;
    const byte *srcEnd = srcPtr + numPixels * 4;
    byte *dstPtr = dst;

    for (; srcPtr < srcEnd; srcPtr += 4, dstPtr += 4) {
        dstPtr[0] = srcPtr[0];
        dstPtr[1] = srcPtr[1];
        dstPtr[2] = srcPtr[2];
        dstPtr[3] = 255;
    }
}
static void BGRX8888ToRGBA8888(const byte *src, byte *dst, int numPixels, bool isGamma) {
    const byte *srcPtr = src;
    const byte *srcEnd = srcPtr + numPixels * 4;
    byte *dstPtr = dst;

    for (; srcPtr < srcEnd; srcPtr += 4, dstPtr += 4) {
        dstPtr[0] = srcPtr[2];
        dstPtr[1] = srcPtr[1];
        dstPtr[2] = srcPtr[0];
        dstPtr[3] = 255;
    }
}
static void RGBA8888ToRGBA8888(const byte *src, byte *dst, int numPixels, bool isGamma) {
    //simdProcessor->Memcpy(dst, src, 4 * numPixels);
    memcpy(dst, src, 4 * numPixels);
}
static void BGRA8888ToRGBA8888(const byte *src, byte *dst, int numPixels, bool isGamma) {
    const byte *srcPtr = src;
    const byte *srcEnd = srcPtr + numPixels * 4;
    byte *dstPtr = dst;

    for (; srcPtr < srcEnd; srcPtr += 4, dstPtr += 4) {
        dstPtr[0] = srcPtr[2];
        dstPtr[1] = srcPtr[1];
        dstPtr[2] = srcPtr[0];
        dstPtr[3] = srcPtr[3];
    }
}
static void ABGR8888ToRGBA8888(const byte *src, byte *dst, int numPixels, bool isGamma) {
    const byte *srcPtr = src;
    const byte *srcEnd = srcPtr + numPixels * 4;
    byte *dstPtr = dst;

    for (; srcPtr < srcEnd; srcPtr += 4, dstPtr += 4) {
        dstPtr[0] = srcPtr[3];
        dstPtr[1] = srcPtr[2];
        dstPtr[2] = srcPtr[1];
        dstPtr[3] = srcPtr[0];
    }
}
static void ARGB8888ToRGBA8888(const byte *src, byte *dst, int numPixels, bool isGamma) {
    const byte *srcPtr = src;
    const byte *srcEnd = srcPtr + numPixels * 4;
    byte *dstPtr = dst;

    for (; srcPtr < srcEnd; srcPtr += 4, dstPtr += 4) {
        dstPtr[0] = srcPtr[1];
        dstPtr[1] = srcPtr[2];
        dstPtr[2] = srcPtr[3];
        dstPtr[3] = srcPtr[0];
    }
}
static void RGBX4444ToRGBA8888(const byte *src, byte *dst, int numPixels, bool isGamma) {
    const uint16_t *srcPtr = (const uint16_t *)src;
    const uint16_t *srcEnd = srcPtr + numPixels;
    byte *dstPtr = dst;

    for (; srcPtr < srcEnd; srcPtr++, dstPtr += 4) {
        dstPtr[0] = ((*srcPtr << 4) & 0xF0) | ((*srcPtr >> 0) & 0x0F);
        dstPtr[1] = ((*srcPtr << 0) & 0xF0) | ((*srcPtr >> 4) & 0x0F);
        dstPtr[2] = ((*srcPtr >> 4) & 0xF0) | ((*srcPtr >> 8) & 0x0F);
        dstPtr[3] = 255;
    }
}
static void BGRX4444ToRGBA8888(const byte *src, byte *dst, int numPixels, bool isGamma) {
    const uint16_t *srcPtr = (const uint16_t *)src;
    const uint16_t *srcEnd = srcPtr + numPixels;
    byte *dstPtr = dst;

    for (; srcPtr < srcEnd; srcPtr++, dstPtr += 4) {
        dstPtr[0] = ((*srcPtr >> 4) & 0xF0) | ((*srcPtr >> 8) & 0x0F);
        dstPtr[1] = ((*srcPtr << 0) & 0xF0) | ((*srcPtr >> 4) & 0x0F);
        dstPtr[2] = ((*srcPtr << 4) & 0xF0) | ((*srcPtr >> 0) & 0x0F);
        dstPtr[3] = 255;
    }
}
static void RGBA4444ToRGBA8888(const byte *src, byte *dst, int numPixels, bool isGamma) {
    const uint16_t *srcPtr = (const uint16_t *)src;
    const uint16_t *srcEnd = srcPtr + numPixels;
    byte *dstPtr = dst;

    for (; srcPtr < srcEnd; srcPtr++, dstPtr += 4) {
        dstPtr[0] = ((*srcPtr << 4) & 0xF0) | ((*srcPtr >> 0) & 0x0F);
        dstPtr[1] = ((*srcPtr << 0) & 0xF0) | ((*srcPtr >> 4) & 0x0F);
        dstPtr[2] = ((*srcPtr >> 4) & 0xF0) | ((*srcPtr >> 8) & 0x0F);
        dstPtr[3] = ((*srcPtr >> 8) & 0xF0) | ((*srcPtr >> 12) & 0x0F);
    }
}
static void BGRA4444ToRGBA8888(const byte *src, byte *dst, int numPixels, bool isGamma) {
    const uint16_t *srcPtr = (const uint16_t *)src;
    const uint16_t *srcEnd = srcPtr + numPixels;
    byte *dstPtr = dst;

    for (; srcPtr < srcEnd; srcPtr++, dstPtr += 4) {
        dstPtr[0] = ((*srcPtr >> 4) & 0xF0) | ((*srcPtr >> 8) & 0x0F);
        dstPtr[1] = ((*srcPtr << 0) & 0xF0) | ((*srcPtr >> 4) & 0x0F);
        dstPtr[2] = ((*srcPtr << 4) & 0xF0) | ((*srcPtr >> 0) & 0x0F);
        dstPtr[3] = ((*srcPtr >> 8) & 0xF0) | ((*srcPtr >> 12) & 0x0F);
    }
}
static void ABGR4444ToRGBA8888(const byte *src, byte *dst, int numPixels, bool isGamma) {
    const uint16_t *srcPtr = (const uint16_t *)src;
    const uint16_t *srcEnd = srcPtr + numPixels;
    byte *dstPtr = dst;

    for (; srcPtr < srcEnd; srcPtr++, dstPtr += 4) {
        dstPtr[0] = ((*srcPtr >> 8) & 0xF0) | ((*srcPtr >> 12) & 0x0F);
        dstPtr[1] = ((*srcPtr >> 4) & 0xF0) | ((*srcPtr >> 8) & 0x0F);
        dstPtr[2] = ((*srcPtr << 0) & 0xF0) | ((*srcPtr >> 4) & 0x0F);
        dstPtr[3] = ((*srcPtr << 4) & 0xF0) | ((*srcPtr >> 0) & 0x0F);
    }
}
static void ARGB4444ToRGBA8888(const byte *src, byte *dst, int numPixels, bool isGamma) {
    const uint16_t *srcPtr = (const uint16_t *)src;
    const uint16_t *srcEnd = srcPtr + numPixels;
    byte *dstPtr = dst;

    for (; srcPtr < srcEnd; srcPtr++, dstPtr += 4) {
        dstPtr[0] = ((*srcPtr << 0) & 0xF0) | ((*srcPtr >> 4) & 0x0F);
        dstPtr[1] = ((*srcPtr >> 4) & 0xF0) | ((*srcPtr >> 8) & 0x0F);
        dstPtr[2] = ((*srcPtr >> 8) & 0xF0) | ((*srcPtr >> 12) & 0x0F);
        dstPtr[3] = ((*srcPtr << 4) & 0xF0) | ((*srcPtr >> 0) & 0x0F);
    }
}
static void RGBX5551ToRGBA8888(const byte *src, byte *dst, int numPixels, bool isGamma) {
    const uint16_t *srcPtr = (const uint16_t *)src;
    const uint16_t *srcEnd = srcPtr + numPixels;
    byte *dstPtr = dst;

    for (; srcPtr < srcEnd; srcPtr++, dstPtr += 4) {
        dstPtr[0] = ((*srcPtr << 3) & 0xF8) | ((*srcPtr >> 2) & 0x7);
        dstPtr[1] = ((*srcPtr >> 2) & 0xF8) | ((*srcPtr >> 7) & 0x7);
        dstPtr[2] = ((*srcPtr >> 7) & 0xF8) | ((*srcPtr >> 12) & 0x7);
        dstPtr[3] = 255;
    }
}
static void BGRX5551ToRGBA8888(const byte *src, byte *dst, int numPixels, bool isGamma) {
    const uint16_t *srcPtr = (const uint16_t *)src;
    const uint16_t *srcEnd = srcPtr + numPixels;
    byte *dstPtr = dst;

    for (; srcPtr < srcEnd; srcPtr++, dstPtr += 4) {
        dstPtr[0] = ((*srcPtr >> 7) & 0xF8) | ((*srcPtr >> 12) & 0x7);
        dstPtr[1] = ((*srcPtr >> 2) & 0xF8) | ((*srcPtr >> 7) & 0x7);
        dstPtr[2] = ((*srcPtr << 3) & 0xF8) | ((*srcPtr >> 2) & 0x7);
        dstPtr[3] = 255;
    }
}
static void RGBA5551ToRGBA8888(const byte *src, byte *dst, int numPixels, bool isGamma) {
    const uint16_t *srcPtr = (const uint16_t *)src;
    const uint16_t *srcEnd = srcPtr + numPixels;
    byte *dstPtr = dst;

    for (; srcPtr < srcEnd; srcPtr++, dstPtr += 4) {
        dstPtr[0] = ((*srcPtr << 3) & 0xF8) | ((*srcPtr >> 2) & 0x7);
        dstPtr[1] = ((*srcPtr >> 2) & 0xF8) | ((*srcPtr >> 7) & 0x7);
        dstPtr[2] = ((*srcPtr >> 7) & 0xF8) | ((*srcPtr >> 12) & 0x7);
        dstPtr[3] = ((int16_t)*srcPtr >> 15) & 0xFF;
    }
}
static void BGRA5551ToRGBA8888(const byte *src, byte *dst, int numPixels, bool isGamma) {
    const uint16_t *srcPtr = (const uint16_t *)src;
    const uint16_t *srcEnd = srcPtr + numPixels;
    byte *dstPtr = dst;

    for (; srcPtr < srcEnd; srcPtr++, dstPtr += 4) {
        dstPtr[0] = ((*srcPtr >> 7) & 0xF8) | ((*srcPtr >> 12) & 0x7);
        dstPtr[1] = ((*srcPtr >> 2) & 0xF8) | ((*srcPtr >> 7) & 0x7);
        dstPtr[2] = ((*srcPtr << 3) & 0xF8) | ((*srcPtr >> 2) & 0x7);
        dstPtr[3] = ((int16_t)*srcPtr >> 15) & 0xFF;
    }
}
static void ABGR1555ToRGBA8888(const byte *src, byte *dst, int numPixels, bool isGamma) {
    const uint16_t *srcPtr = (const uint16_t *)src;
    const uint16_t *srcEnd = srcPtr + numPixels;
    byte *dstPtr = dst;

    for (; srcPtr < srcEnd; srcPtr++, dstPtr += 4) {
        dstPtr[0] = ((*srcPtr >> 8) & 0xF8) | ((*srcPtr >> 13) & 0x7);
        dstPtr[1] = ((*srcPtr >> 3) & 0xF8) | ((*srcPtr >> 8) & 0x7);
        dstPtr[2] = ((*srcPtr << 2) & 0xF8) | ((*srcPtr >> 3) & 0x7);
        dstPtr[3] = (*srcPtr & 1) ? 0xFF : 0;
    }
}
static void ARGB1555ToRGBA8888(const byte *src, byte *dst, int numPixels, bool isGamma) {
    const uint16_t *srcPtr = (const uint16_t *)src;
    const uint16_t *srcEnd = srcPtr + numPixels;
    byte *dstPtr = dst;

    for (; srcPtr < srcEnd; srcPtr++, dstPtr += 4) {
        dstPtr[0] = ((*srcPtr << 2) & 0xF8) | ((*srcPtr >> 3) & 0x7);
        dstPtr[1] = ((*srcPtr >> 3) & 0xF8) | ((*srcPtr >> 8) & 0x7);
        dstPtr[2] = ((*srcPtr >> 8) & 0xF8) | ((*srcPtr >> 13) & 0x7);
        dstPtr[3] = (*srcPtr & 1) ? 0xFF : 0;
    }
}
static void RGB565ToRGBA8888(const byte *src, byte *dst, int numPixels, bool isGamma) {
    const uint16_t *srcPtr = (const uint16_t *)src;
    const uint16_t *srcEnd = srcPtr + numPixels;
    byte *dstPtr = dst;

    for (; srcPtr < srcEnd; srcPtr++, dstPtr += 4) {
        dstPtr[0] = ((*srcPtr << 3) & 0xF8) | ((*srcPtr >> 2) & 0x7);
        dstPtr[1] = ((*srcPtr >> 3) & 0xFC) | ((*srcPtr >> 9) & 0x3);
        dstPtr[2] = ((*srcPtr >> 8) & 0xF8) | ((*srcPtr >> 14) & 0x7);
        dstPtr[3] = 255;
    }
}
static void BGR565ToRGBA8888(const byte *src, byte *dst, int numPixels, bool isGamma) {
    const uint16_t *srcPtr = (const uint16_t *)src;
    const uint16_t *srcEnd = srcPtr + numPixels;
    byte *dstPtr = dst;

    for (; srcPtr < srcEnd; srcPtr++, dstPtr += 4) {
        dstPtr[0] = ((*srcPtr >> 8) & 0xF8) | ((*srcPtr >> 14) & 0x7);
        dstPtr[1] = ((*srcPtr >> 3) & 0xFC) | ((*srcPtr >> 9) & 0x3);
        dstPtr[2] = ((*srcPtr << 3) & 0xF8) | ((*srcPtr >> 2) & 0x7);
        dstPtr[3] = 255;
    }
}
static void RGBE9995ToRGBA8888(const byte *src, byte *dst, int numPixels, bool isGamma) {
    const uint32_t *srcPtr = (const uint32_t *)src;
    const uint32_t *srcEnd = srcPtr + numPixels;
    byte *dstPtr = dst;
    float m;

    if (isGamma) {
        for (; srcPtr < srcEnd; srcPtr++, dstPtr += 4) {
            m = Math::Pow(2, ((*srcPtr >> 27) & 0x1F) - 24);
            dstPtr[0] = Math::Ftob(255.0f * Image::LinearToGammaFast(m * (*srcPtr & 0x1FF)));
            dstPtr[1] = Math::Ftob(255.0f * Image::LinearToGammaFast(m * ((*srcPtr >> 9) & 0x1FF)));
            dstPtr[2] = Math::Ftob(255.0f * Image::LinearToGammaFast(m * ((*srcPtr >> 18) & 0x1FF)));
            dstPtr[3] = 255;
        }
    } else {
        for (; srcPtr < srcEnd; srcPtr++, dstPtr += 4) {
            m = Math::Pow(2, ((*srcPtr >> 27) & 0x1F) - 24);
            dstPtr[0] = Math::Ftob(255.0f * m * (*srcPtr & 0x1FF));
            dstPtr[1] = Math::Ftob(255.0f * m * ((*srcPtr >> 9) & 0x1FF));
            dstPtr[2] = Math::Ftob(255.0f * m * ((*srcPtr >> 18) & 0x1FF));
            dstPtr[3] = 255;
        }
    }
}
static void L16FToRGBA8888(const byte *src, byte *dst, int numPixels, bool isGamma) {
    const float16_t *srcPtr = (const float16_t *)src;
    const float16_t *srcEnd = srcPtr + numPixels;
    byte *dstPtr = dst;

    if (isGamma) {
        for (; srcPtr < srcEnd; srcPtr += 1, dstPtr += 4) {
            dstPtr[0] = dstPtr[1] = dstPtr[2] = Math::Ftob(255.0f * Image::LinearToGammaFast(F16Converter::ToF32(srcPtr[0])));
            dstPtr[3] = 255;
        }
    } else {
        for (; srcPtr < srcEnd; srcPtr += 1, dstPtr += 4) {
            dstPtr[0] = dstPtr[1] = dstPtr[2] = Math::Ftob(255.0f * F16Converter::ToF32(srcPtr[0]));
            dstPtr[3] = 255;
        }
    }
}
static void A16FToRGBA8888(const byte *src, byte *dst, int numPixels, bool isGamma) {
    const float16_t *srcPtr = (const float16_t *)src;
    const float16_t *srcEnd = srcPtr + numPixels;
    byte *dstPtr = dst;

    for (; srcPtr < srcEnd; srcPtr += 1, dstPtr += 4) {
        dstPtr[0] = 255;
        dstPtr[1] = 255;
        dstPtr[2] = 255;
        dstPtr[3] = Math::Ftob(255.0f * F16Converter::ToF32(srcPtr[0]));
    }
}
static void LA16FToRGBA8888(const byte *src, byte *dst, int numPixels, bool isGamma) {
    const float16_t *srcEnd = (const float16_t *)src + numPixels * 2;
    const float16_t *srcPtr = (const float16_t *)src;
    byte *dstPtr = dst;

    if (isGamma) {
        for (; srcPtr < srcEnd; srcPtr += 2, dstPtr += 4) {
            dstPtr[0] = dstPtr[1] = dstPtr[2] = Math::Ftob(255.0f * Image::LinearToGammaFast(F16Converter::ToF32(srcPtr[0])));
            dstPtr[3] = Math::Ftob(255.0f * F16Converter::ToF32(srcPtr[1]));
        }
    } else {
        for (; srcPtr < srcEnd; srcPtr += 2, dstPtr += 4) {
            dstPtr[0] = dstPtr[1] = dstPtr[2] = Math::Ftob(255.0f * F16Converter::ToF32(srcPtr[0]));
            dstPtr[3] = Math::Ftob(255.0f * F16Converter::ToF32(srcPtr[1]));
        }
    }
}
static void R16FToRGBA8888(const byte *src, byte *dst, int numPixels, bool isGamma) {
    const float16_t *srcPtr = (const float16_t *)src;
    const float16_t *srcEnd = srcPtr + numPixels;
    byte *dstPtr = dst;

    if (isGamma) {
        for (; srcPtr < srcEnd; srcPtr += 1, dstPtr += 4) {
            dstPtr[0] = Math::Ftob(255.0f * Image::LinearToGammaFast(F16Converter::ToF32(srcPtr[0])));
            dstPtr[1] = 0;
            dstPtr[2] = 0;
            dstPtr[3] = 255;
        }
    } else {
        for (; srcPtr < srcEnd; srcPtr += 1, dstPtr += 4) {
            dstPtr[0] = Math::Ftob(255.0f * F16Converter::ToF32(srcPtr[0]));
            dstPtr[1] = 0;
            dstPtr[2] = 0;
            dstPtr[3] = 255;
        }
    }
}
static void RG16FToRGBA8888(const byte *src, byte *dst, int numPixels, bool isGamma) {
    const float16_t *srcEnd = (const float16_t *)src + numPixels * 2;
    const float16_t *srcPtr = (const float16_t *)src;
    byte *dstPtr = dst;

    if (isGamma) {
        for (; srcPtr < srcEnd; srcPtr += 2, dstPtr += 4) {
            dstPtr[0] = Math::Ftob(255.0f * Image::LinearToGammaFast(F16Converter::ToF32(srcPtr[0])));
            dstPtr[1] = Math::Ftob(255.0f * Image::LinearToGammaFast(F16Converter::ToF32(srcPtr[1])));
            dstPtr[2] = 0;
            dstPtr[3] = 255;
        }
    } else {
        for (; srcPtr < srcEnd; srcPtr += 2, dstPtr += 4) {
            dstPtr[0] = Math::Ftob(255.0f * F16Converter::ToF32(srcPtr[0]));
            dstPtr[1] = Math::Ftob(255.0f * F16Converter::ToF32(srcPtr[1]));
            dstPtr[2] = 0;
            dstPtr[3] = 255;
        }
    }
}
static void RGB16FToRGBA8888(const byte *src, byte *dst, int numPixels, bool isGamma) {
    const float16_t *srcPtr = (const float16_t *)src;
    const float16_t *srcEnd = srcPtr + numPixels * 3;
    byte *dstPtr = dst;

    if (isGamma) {
        for (; srcPtr < srcEnd; srcPtr += 3, dstPtr += 4) {
            dstPtr[0] = Math::Ftob(255.0f * Image::LinearToGammaFast(F16Converter::ToF32(srcPtr[0])));
            dstPtr[1] = Math::Ftob(255.0f * Image::LinearToGammaFast(F16Converter::ToF32(srcPtr[1])));
            dstPtr[2] = Math::Ftob(255.0f * Image::LinearToGammaFast(F16Converter::ToF32(srcPtr[2])));
            dstPtr[3] = 255;
        }
    } else {
        for (; srcPtr < srcEnd; srcPtr += 3, dstPtr += 4) {
            dstPtr[0] = Math::Ftob(255.0f * F16Converter::ToF32(srcPtr[0]));
            dstPtr[1] = Math::Ftob(255.0f * F16Converter::ToF32(srcPtr[1]));
            dstPtr[2] = Math::Ftob(255.0f * F16Converter::ToF32(srcPtr[2]));
            dstPtr[3] = 255;
        }
    }
}
static void RGBA16FToRGBA8888(const byte *src, byte *dst, int numPixels, bool isGamma) {
    const float16_t *srcPtr = (const float16_t *)src;
    const float16_t *srcEnd = srcPtr + numPixels * 4;
    byte *dstPtr = dst;

    if (isGamma) {
        for (; srcPtr < srcEnd; srcPtr += 4, dstPtr += 4) {
            dstPtr[0] = Math::Ftob(255.0f * Image::LinearToGammaFast(F16Converter::ToF32(srcPtr[0])));
            dstPtr[1] = Math::Ftob(255.0f * Image::LinearToGammaFast(F16Converter::ToF32(srcPtr[1])));
            dstPtr[2] = Math::Ftob(255.0f * Image::LinearToGammaFast(F16Converter::ToF32(srcPtr[2])));
            dstPtr[3] = Math::Ftob(255.0f * F16Converter::ToF32(srcPtr[3]));
        }
    } else {
        for (; srcPtr < srcEnd; srcPtr += 4, dstPtr += 4) {
            dstPtr[0] = Math::Ftob(255.0f * F16Converter::ToF32(srcPtr[0]));
            dstPtr[1] = Math::Ftob(255.0f * F16Converter::ToF32(srcPtr[1]));
            dstPtr[2] = Math::Ftob(255.0f * F16Converter::ToF32(srcPtr[2]));
            dstPtr[3] = Math::Ftob(255.0f * F16Converter::ToF32(srcPtr[3]));
        }
    }
}
static void L32FToRGBA8888(const byte *src, byte *dst, int numPixels, bool isGamma) {
    const float *srcPtr = (const float *)src;
    const float *srcEnd = srcPtr + numPixels;
    byte *dstPtr = dst;

    if (isGamma) {
        for (; srcPtr < srcEnd; srcPtr += 1, dstPtr += 4) {
            dstPtr[0] = dstPtr[1] = dstPtr[2] = Math::Ftob(255.0f * Image::LinearToGammaFast(srcPtr[0]));
            dstPtr[3] = 255;
        }
    } else {
        for (; srcPtr < srcEnd; srcPtr += 1, dstPtr += 4) {
            dstPtr[0] = dstPtr[1] = dstPtr[2] = Math::Ftob(255.0f * srcPtr[0]);
            dstPtr[3] = 255;
        }
    }
}

static void A32FToRGBA8888(const byte *src, byte *dst, int numPixels, bool isGamma) {
    const float *srcPtr = (const float *)src;
    const float *srcEnd = srcPtr + numPixels;
    byte *dstPtr = dst;

    for (; srcPtr < srcEnd; srcPtr += 1, dstPtr += 4) {
        dstPtr[0] = 255;
        dstPtr[1] = 255;
        dstPtr[2] = 255;
        dstPtr[3] = Math::Ftob(255.0f * srcPtr[0]);
    }
}
static void LA32FToRGBA8888(const byte *src, byte *dst, int numPixels, bool isGamma) {
    const float *srcPtr = (const float *)src;
    const float *srcEnd = srcPtr + numPixels * 2;
    byte *dstPtr = dst;

    if (isGamma) {
        for (; srcPtr < srcEnd; srcPtr += 2, dstPtr += 4) {
            dstPtr[0] = dstPtr[1] = dstPtr[2] = Math::Ftob(255.0f * Image::LinearToGammaFast(srcPtr[0]));
            dstPtr[3] = Math::Ftob(255.0f * srcPtr[1]);
        }
    } else {
        for (; srcPtr < srcEnd; srcPtr += 2, dstPtr += 4) {
            dstPtr[0] = dstPtr[1] = dstPtr[2] = Math::Ftob(255.0f * srcPtr[0]);
            dstPtr[3] = Math::Ftob(255.0f * srcPtr[1]);
        }
    }
}
static void R32FToRGBA8888(const byte *src, byte *dst, int numPixels, bool isGamma) {
    const float *srcPtr = (const float *)src;
    const float *srcEnd = srcPtr + numPixels;
    byte *dstPtr = dst;

    if (isGamma) {
        for (; srcPtr < srcEnd; srcPtr += 1, dstPtr += 4) {
            dstPtr[0] = Math::Ftob(255.0f * Image::LinearToGammaFast(srcPtr[0]));
            dstPtr[1] = 0;
            dstPtr[2] = 0;
            dstPtr[3] = 255;
        }
    } else {
        for (; srcPtr < srcEnd; srcPtr += 1, dstPtr += 4) {
            dstPtr[0] = Math::Ftob(255.0f * srcPtr[0]);
            dstPtr[1] = 0;
            dstPtr[2] = 0;
            dstPtr[3] = 255;
        }
    }
}
static void RG32FToRGBA8888(const byte *src, byte *dst, int numPixels, bool isGamma) {
    const float *srcPtr = (const float *)src;
    const float *srcEnd = srcPtr + numPixels * 2;
    byte *dstPtr = dst;

    if (isGamma) {
        for (; srcPtr < srcEnd; srcPtr += 2, dstPtr += 4) {
            dstPtr[0] = Math::Ftob(255.0f * Image::LinearToGammaFast(srcPtr[0]));
            dstPtr[1] = Math::Ftob(255.0f * Image::LinearToGammaFast(srcPtr[1]));
            dstPtr[2] = 0;
            dstPtr[3] = 255;
        }
    } else {
        for (; srcPtr < srcEnd; srcPtr += 2, dstPtr += 4) {
            dstPtr[0] = Math::Ftob(255.0f * srcPtr[0]);
            dstPtr[1] = Math::Ftob(255.0f * srcPtr[1]);
            dstPtr[2] = 0;
            dstPtr[3] = 255;
        }
    }
}
static void RGB32FToRGBA8888(const byte *src, byte *dst, int numPixels, bool isGamma) {
    const float *srcPtr = (const float *)src;
    const float *srcEnd = srcPtr + numPixels * 3;
    byte *dstPtr = dst;

    if (isGamma) {
        for (; srcPtr < srcEnd; srcPtr += 3, dstPtr += 4) {
            dstPtr[0] = Math::Ftob(255.0f * Image::LinearToGammaFast(srcPtr[0]));
            dstPtr[1] = Math::Ftob(255.0f * Image::LinearToGammaFast(srcPtr[1]));
            dstPtr[2] = Math::Ftob(255.0f * Image::LinearToGammaFast(srcPtr[2]));
            dstPtr[3] = 255;
        }
    } else {
        for (; srcPtr < srcEnd; srcPtr += 3, dstPtr += 4) {
            dstPtr[0] = Math::Ftob(255.0f * srcPtr[0]);
            dstPtr[1] = Math::Ftob(255.0f * srcPtr[1]);
            dstPtr[2] = Math::Ftob(255.0f * srcPtr[2]);
            dstPtr[3] = 255;
        }
    }
}
static void RGBA32FToRGBA8888(const byte *src, byte *dst, int numPixels, bool isGamma) {
    const float *srcPtr = (const float *)src;
    const float *srcEnd = srcPtr + numPixels * 4;
    byte *dstPtr = dst;

    if (isGamma) {
        for (; srcPtr < srcEnd; srcPtr += 4, dstPtr += 4) {
            dstPtr[0] = Math::Ftob(255.0f * Image::LinearToGammaFast(srcPtr[0]));
            dstPtr[1] = Math::Ftob(255.0f * Image::LinearToGammaFast(srcPtr[1]));
            dstPtr[2] = Math::Ftob(255.0f * Image::LinearToGammaFast(srcPtr[2]));
            dstPtr[3] = Math::Ftob(255.0f * Image::LinearToGammaFast(srcPtr[3]));
        }
    } else {
        for (; srcPtr < srcEnd; srcPtr += 4, dstPtr += 4) {
            dstPtr[0] = Math::Ftob(255.0f * srcPtr[0]);
            dstPtr[1] = Math::Ftob(255.0f * srcPtr[1]);
            dstPtr[2] = Math::Ftob(255.0f * srcPtr[2]);
            dstPtr[3] = Math::Ftob(255.0f * srcPtr[3]);
        }
    }
}
static void RGB11F11F10FToRGBA8888(const byte *src, byte *dst, int numPixels, bool isGamma) {
    const uint32_t *srcPtr = (const uint32_t *)src;
    const uint32_t *srcEnd = srcPtr + numPixels;
    byte *dstPtr = dst;

    if (isGamma) {
        for (; srcPtr < srcEnd; srcPtr += 1, dstPtr += 4) {
            dstPtr[0] = Math::Ftob(255.0f * Image::LinearToGammaFast(F11Converter::ToF32(srcPtr[0] & 0x7FF)));
            dstPtr[1] = Math::Ftob(255.0f * Image::LinearToGammaFast(F11Converter::ToF32((srcPtr[1] >> 11) & 0x7FF)));
            dstPtr[2] = Math::Ftob(255.0f * Image::LinearToGammaFast(F10Converter::ToF32((srcPtr[2] >> 22) & 0x3FF)));
            dstPtr[3] = 255;
        }
    } else {
        for (; srcPtr < srcEnd; srcPtr += 1, dstPtr += 4) {
            dstPtr[0] = Math::Ftob(255.0f * F11Converter::ToF32(srcPtr[0] & 0x7FF));
            dstPtr[1] = Math::Ftob(255.0f * F11Converter::ToF32((srcPtr[1] >> 11) & 0x7FF));
            dstPtr[2] = Math::Ftob(255.0f * F10Converter::ToF32((srcPtr[2] >> 22) & 0x3FF));
            dstPtr[3] = 255;
        }
    }
}

//--------------------------------------------------------------------------------------------------
//
// RGBA8888ToXXX (packing function from rgba8888 to custom format)
//
//--------------------------------------------------------------------------------------------------

static void RGBA8888ToL8(const byte *src, byte *dst, int numPixels, bool isGamma) {
    const byte *srcPtr = src;
    const byte *srcEnd = srcPtr + numPixels * 4;
    byte *dstPtr = dst;

    for (; srcPtr < srcEnd; srcPtr += 4, dstPtr += 1) {
        dstPtr[0] = (byte)(0.299f * srcPtr[0] + 0.587f * srcPtr[1] + 0.114f * srcPtr[2]);
    }
}
static void RGBA8888ToA8(const byte *src, byte *dst, int numPixels, bool isGamma) {
    const byte *srcPtr = src;
    const byte *srcEnd = srcPtr + numPixels * 4;
    byte *dstPtr = dst;

    for (; srcPtr < srcEnd; srcPtr += 4, dstPtr += 1) {
        dstPtr[0] = srcPtr[3];
    }
}
static void RGBA8888ToLA88(const byte *src, byte *dst, int numPixels, bool isGamma) {
    const byte *srcPtr = src;
    const byte *srcEnd = srcPtr + numPixels * 4;
    byte *dstPtr = dst;

    for (; srcPtr < srcEnd; srcPtr += 4, dstPtr += 2) {
        dstPtr[0] = (byte)(0.299f * srcPtr[0] + 0.587f * srcPtr[1] + 0.114f * srcPtr[2]);
        dstPtr[1] = srcPtr[3];
    }
}
static void RGBA8888ToLA1616(const byte *src, byte *dst, int numPixels, bool isGamma) {
    const byte *srcPtr = src;
    const byte *srcEnd = srcPtr + numPixels * 4;
    uint16_t *dstPtr = (uint16_t *)dst;

    for (; srcPtr < srcEnd; srcPtr += 4, dstPtr += 2) {
        dstPtr[0] = (((uint16_t)(0.299f * srcPtr[0] + 0.587f * srcPtr[1] + 0.114f * srcPtr[2])) << 8);
        dstPtr[1] = (((uint16_t)srcPtr[3]) << 8);
    }
}
static void RGBA8888ToR8(const byte *src, byte *dst, int numPixels, bool isGamma) {
    const byte *srcPtr = src;
    const byte *srcEnd = srcPtr + numPixels * 4;
    byte *dstPtr = dst;

    for (; srcPtr < srcEnd; srcPtr += 4, dstPtr += 1) {
        dstPtr[0] = srcPtr[0];
    }
}
static void RGBA8888ToRG88(const byte *src, byte *dst, int numPixels, bool isGamma) {
    const byte *srcPtr = src;
    const byte *srcEnd = srcPtr + numPixels * 4;
    byte *dstPtr = dst;

    for (; srcPtr < srcEnd; srcPtr += 4, dstPtr += 2) {
        dstPtr[0] = srcPtr[0];
        dstPtr[1] = srcPtr[1];
    }
}
static void RGBA8888ToRGB888(const byte *src, byte *dst, int numPixels, bool isGamma) {
    const byte *srcPtr = src;
    const byte *srcEnd = srcPtr + numPixels * 4;
    byte *dstPtr = dst;

    for (; srcPtr < srcEnd; srcPtr += 4, dstPtr += 3) {
        dstPtr[0] = srcPtr[0];
        dstPtr[1] = srcPtr[1];
        dstPtr[2] = srcPtr[2];
    }
}
static void RGBA8888ToBGR888(const byte *src, byte *dst, int numPixels, bool isGamma) {
    const byte *srcPtr = src;
    const byte *srcEnd = srcPtr + numPixels * 4;
    byte *dstPtr = dst;

    for (; srcPtr < srcEnd; srcPtr += 4, dstPtr += 3) {
        dstPtr[0] = srcPtr[2];
        dstPtr[1] = srcPtr[1];
        dstPtr[2] = srcPtr[0];
    }
}
static void RGBA8888ToRGBX8888(const byte *src, byte *dst, int numPixels, bool isGamma) {
    const byte *srcPtr = src;
    const byte *srcEnd = srcPtr + numPixels * 4;
    byte *dstPtr = dst;

    for (; srcPtr < srcEnd; srcPtr += 4, dstPtr += 4) {
        dstPtr[0] = srcPtr[0];
        dstPtr[1] = srcPtr[1];
        dstPtr[2] = srcPtr[2];
        dstPtr[3] = 0;
    }
}
static void RGBA8888ToBGRX8888(const byte *src, byte *dst, int numPixels, bool isGamma) {
    const byte *srcPtr = src;
    const byte *srcEnd = srcPtr + numPixels * 4;
    byte *dstPtr = dst;

    for (; srcPtr < srcEnd; srcPtr += 4, dstPtr += 4) {
        dstPtr[0] = srcPtr[2];
        dstPtr[1] = srcPtr[1];
        dstPtr[2] = srcPtr[0];
        dstPtr[3] = 0;
    }
}
static void RGBA8888ToBGRA8888(const byte *src, byte *dst, int numPixels, bool isGamma) {
    const byte *srcPtr = src;
    const byte *srcEnd = srcPtr + numPixels * 4;
    byte *dstPtr = dst;

    for (; srcPtr < srcEnd; srcPtr += 4, dstPtr += 4) {
        dstPtr[0] = srcPtr[2];
        dstPtr[1] = srcPtr[1];
        dstPtr[2] = srcPtr[0];
        dstPtr[3] = srcPtr[3];
    }
}
static void RGBA8888ToABGR8888(const byte *src, byte *dst, int numPixels, bool isGamma) {
    const byte *srcPtr = src;
    const byte *srcEnd = srcPtr + numPixels * 4;
    byte *dstPtr = dst;

    for (; srcPtr < srcEnd; srcPtr += 4, dstPtr += 4) {
        dstPtr[0] = srcPtr[3];
        dstPtr[1] = srcPtr[2];
        dstPtr[2] = srcPtr[1];
        dstPtr[3] = srcPtr[0];
    }
}
static void RGBA8888ToARGB8888(const byte *src, byte *dst, int numPixels, bool isGamma) {
    const byte *srcPtr = src;
    const byte *srcEnd = srcPtr + numPixels * 4;
    byte *dstPtr = dst;

    for (; srcPtr < srcEnd; srcPtr += 4, dstPtr += 4) {
        dstPtr[0] = srcPtr[1];
        dstPtr[1] = srcPtr[2];
        dstPtr[2] = srcPtr[3];
        dstPtr[3] = srcPtr[0];
    }
}
static void RGBA8888ToRGBX4444(const byte *src, byte *dst, int numPixels, bool isGamma) {
    const byte *srcPtr = src;
    const byte *srcEnd = srcPtr + numPixels * 4;
    uint16_t *dstPtr = (uint16_t *)dst;

    for (; srcPtr < srcEnd; srcPtr += 4, dstPtr++) {
        *dstPtr = (srcPtr[0] >> 4) | ((srcPtr[1] >> 4) << 4) | ((srcPtr[2] >> 4) << 8);
    }
}
static void RGBA8888ToBGRX4444(const byte *src, byte *dst, int numPixels, bool isGamma) {
    const byte *srcPtr = src;
    const byte *srcEnd = srcPtr + numPixels * 4;
    uint16_t *dstPtr = (uint16_t *)dst;

    for (; srcPtr < srcEnd; srcPtr += 4, dstPtr++) {
        *dstPtr = (srcPtr[2] >> 4) | ((srcPtr[1] >> 4) << 4) | ((srcPtr[0] >> 4) << 8);
    }
}
static void RGBA8888ToRGBA4444(const byte *src, byte *dst, int numPixels, bool isGamma) {
    const byte *srcPtr = src;
    const byte *srcEnd = srcPtr + numPixels * 4;
    uint16_t *dstPtr = (uint16_t *)dst;

    for (; srcPtr < srcEnd; srcPtr += 4, dstPtr++) {
        *dstPtr = (srcPtr[0] >> 4) | ((srcPtr[1] >> 4) << 4) | ((srcPtr[2] >> 4) << 8) | ((srcPtr[3] >> 4) << 12);
    }
}
static void RGBA8888ToBGRA4444(const byte *src, byte *dst, int numPixels, bool isGamma) {
    const byte *srcPtr = src;
    const byte *srcEnd = srcPtr + numPixels * 4;
    uint16_t *dstPtr = (uint16_t *)dst;

    for (; srcPtr < srcEnd; srcPtr += 4, dstPtr++) {
        *dstPtr = (srcPtr[2] >> 4) | ((srcPtr[1] >> 4) << 4) | ((srcPtr[0] >> 4) << 8) | ((srcPtr[3] >> 4) << 12);
    }
}
static void RGBA8888ToABGR4444(const byte *src, byte *dst, int numPixels, bool isGamma) {
    const byte *srcPtr = src;
    const byte *srcEnd = srcPtr + numPixels * 4;
    uint16_t *dstPtr = (uint16_t *)dst;

    for (; srcPtr < srcEnd; srcPtr += 4, dstPtr++) {
        *dstPtr = (srcPtr[3] >> 4) | ((srcPtr[2] >> 4) << 4) | ((srcPtr[1] >> 4) << 8) | ((srcPtr[0] >> 4) << 12);
    }
}
static void RGBA8888ToARGB4444(const byte *src, byte *dst, int numPixels, bool isGamma) {
    const byte *srcPtr = src;
    const byte *srcEnd = srcPtr + numPixels * 4;
    uint16_t *dstPtr = (uint16_t *)dst;

    for (; srcPtr < srcEnd; srcPtr += 4, dstPtr++) {
        *dstPtr = (srcPtr[1] >> 4) | ((srcPtr[2] >> 4) << 4) | ((srcPtr[3] >> 4) << 8) | ((srcPtr[0] >> 4) << 12);
    }
}
static void RGBA8888ToRGBX5551(const byte *src, byte *dst, int numPixels, bool isGamma) {
    const byte *srcPtr = src;
    const byte *srcEnd = srcPtr + numPixels * 4;
    uint16_t *dstPtr = (uint16_t *)dst;

    for (; srcPtr < srcEnd; srcPtr += 4, dstPtr++) {
        *dstPtr = (srcPtr[0] >> 3) | ((srcPtr[1] >> 3) << 5) | ((srcPtr[2] >> 3) << 10);
    }
}
static void RGBA8888ToBGRX5551(const byte *src, byte *dst, int numPixels, bool isGamma) {
    const byte *srcPtr = src;
    const byte *srcEnd = srcPtr + numPixels * 4;
    uint16_t *dstPtr = (uint16_t *)dst;

    for (; srcPtr < srcEnd; srcPtr += 4, dstPtr++) {
        *dstPtr = (srcPtr[2] >> 3) | ((srcPtr[1] >> 3) << 5) | ((srcPtr[0] >> 3) << 10);
    }
}
static void RGBA8888ToRGBA5551(const byte *src, byte *dst, int numPixels, bool isGamma) {
    const byte *srcPtr = src;
    const byte *srcEnd = srcPtr + numPixels * 4;
    uint16_t *dstPtr = (uint16_t *)dst;

    for (; srcPtr < srcEnd; srcPtr += 4, dstPtr++) {
        *dstPtr = (srcPtr[0] >> 3) | ((srcPtr[1] >> 3) << 5) | ((srcPtr[2] >> 3) << 10) | ((srcPtr[3] >> 7) << 15);
    }
}
static void RGBA8888ToBGRA5551(const byte *src, byte *dst, int numPixels, bool isGamma) {
    const byte *srcPtr = src;
    const byte *srcEnd = srcPtr + numPixels * 4;
    uint16_t *dstPtr = (uint16_t *)dst;

    for (; srcPtr < srcEnd; srcPtr += 4, dstPtr++) {
        *dstPtr = (srcPtr[2] >> 3) | ((srcPtr[1] >> 3) << 5) | ((srcPtr[0] >> 3) << 10) | ((srcPtr[3] >> 7) << 15);
    }
}
static void RGBA8888ToABGR1555(const byte *src, byte *dst, int numPixels, bool isGamma) {
    const byte *srcPtr = src;
    const byte *srcEnd = srcPtr + numPixels * 4;
    uint16_t *dstPtr = (uint16_t *)dst;

    for (; srcPtr < srcEnd; srcPtr += 4, dstPtr++) {
        *dstPtr = (srcPtr[3] >> 3) | ((srcPtr[2] >> 3) << 5) | ((srcPtr[1] >> 3) << 10) | ((srcPtr[0] >> 7) << 15);
    }
}
static void RGBA8888ToARGB1555(const byte *src, byte *dst, int numPixels, bool isGamma) {
    const byte *srcPtr = src;
    const byte *srcEnd = srcPtr + numPixels * 4;
    uint16_t *dstPtr = (uint16_t *)dst;

    for (; srcPtr < srcEnd; srcPtr += 4, dstPtr++) {
        *dstPtr = (srcPtr[1] >> 3) | ((srcPtr[2] >> 3) << 5) | ((srcPtr[3] >> 3) << 10) | ((srcPtr[0] >> 7) << 15);
    }
}
static void RGBA8888ToRGB565(const byte *src, byte *dst, int numPixels, bool isGamma) {
    const byte *srcPtr = src;
    const byte *srcEnd = srcPtr + numPixels * 4;
    uint16_t *dstPtr = (uint16_t *)dst;

    for (; srcPtr < srcEnd; srcPtr += 4, dstPtr++) {
        *dstPtr = (srcPtr[0] >> 3) | ((srcPtr[1] >> 2) << 5) | ((srcPtr[2] >> 3) << 11);
    }
}
static void RGBA8888ToBGR565(const byte *src, byte *dst, int numPixels, bool isGamma) {
    const byte *srcPtr = src;
    const byte *srcEnd = srcPtr + numPixels * 4;
    uint16_t *dstPtr = (uint16_t *)dst;

    for (; srcPtr < srcEnd; srcPtr += 4, dstPtr++) {
        *dstPtr = (srcPtr[2] >> 3) | ((srcPtr[1] >> 2) << 5) | ((srcPtr[0] >> 3) << 11);
    }
}
static void RGBA8888ToRGBE9995(const byte *src, byte *dst, int numPixels, bool isGamma) {
    const byte *srcPtr = src;
    const byte *srcEnd = srcPtr + numPixels * 4;
    uint32_t *dstPtr = (uint32_t *)dst;

    if (isGamma) {
        for (; srcPtr < srcEnd; srcPtr += 4, dstPtr++) {
            *dstPtr = RGBE9995::FromColor3(gammaToLinearTable[srcPtr[0]], gammaToLinearTable[srcPtr[1]], gammaToLinearTable[srcPtr[2]]);
        }
    } else {
        for (; srcPtr < srcEnd; srcPtr += 4, dstPtr++) {
            *dstPtr = RGBE9995::FromColor3(srcPtr[0] / 255.0f, srcPtr[1] / 255.0f, srcPtr[2] / 255.0f);
        }
    }
}
static void RGBA8888ToL16F(const byte *src, byte *dst, int numPixels, bool isGamma) {
    const float invNorm = 1.0f / 255.0f;
    const byte *srcPtr = src;
    const byte *srcEnd = srcPtr + numPixels * 4;
    float16_t *dstPtr = (float16_t *)dst;

    if (isGamma) {
        for (; srcPtr < srcEnd; srcPtr += 4, dstPtr += 1) {
            dstPtr[0] = F16Converter::FromF32(Image::GammaToLinearFast(0.299f * (srcPtr[0] * invNorm) + 0.587f * (srcPtr[1] * invNorm) + 0.114f * (srcPtr[2] * invNorm)));
        }
    } else {
        for (; srcPtr < srcEnd; srcPtr += 4, dstPtr += 1) {
            dstPtr[0] = F16Converter::FromF32(0.299f * (srcPtr[0] * invNorm) + 0.587f * (srcPtr[1] * invNorm) + 0.114f * (srcPtr[2] * invNorm));
        }
    }
}
static void RGBA8888ToA16F(const byte *src, byte *dst, int numPixels, bool isGamma) {
    const float invNorm = 1.0f / 255.0f;
    const byte *srcPtr = src;
    const byte *srcEnd = srcPtr + numPixels * 4;
    float16_t *dstPtr = (float16_t *)dst;

    for (; srcPtr < srcEnd; srcPtr += 4, dstPtr += 1) {
        dstPtr[0] = F16Converter::FromF32(srcPtr[3] * invNorm);
    }
}
static void RGBA8888ToLA16F(const byte *src, byte *dst, int numPixels, bool isGamma) {
    const float invNorm = 1.0f / 255.0f;
    const byte *srcPtr = src;
    const byte *srcEnd = srcPtr + numPixels * 4;
    float16_t *dstPtr = (float16_t *)dst;

    if (isGamma) {
        for (; srcPtr < srcEnd; srcPtr += 4, dstPtr += 2) {
            dstPtr[0] = F16Converter::FromF32(Image::GammaToLinearFast(0.299f * (srcPtr[0] * invNorm) + 0.587f * (srcPtr[1] * invNorm) + 0.114f * (srcPtr[2] * invNorm)));
            dstPtr[1] = F16Converter::FromF32(srcPtr[3] * invNorm);
        }
    } else {
        for (; srcPtr < srcEnd; srcPtr += 4, dstPtr += 2) {
            dstPtr[0] = F16Converter::FromF32(0.299f * (srcPtr[0] * invNorm) + 0.587f * (srcPtr[1] * invNorm) + 0.114f * (srcPtr[2] * invNorm));
            dstPtr[1] = F16Converter::FromF32(srcPtr[3] * invNorm);
        }
    }
}
static void RGBA8888ToR16F(const byte *src, byte *dst, int numPixels, bool isGamma) {
    const float invNorm = 1.0f / 255.0f;
    const byte *srcPtr = src;
    const byte *srcEnd = srcPtr + numPixels * 4;
    float16_t *dstPtr = (float16_t *)dst;

    if (isGamma) {
        for (; srcPtr < srcEnd; srcPtr += 4, dstPtr += 1) {
            dstPtr[0] = F16Converter::FromF32(gammaToLinearTable[srcPtr[0]]);
        }
    } else {
        for (; srcPtr < srcEnd; srcPtr += 4, dstPtr += 1) {
            dstPtr[0] = F16Converter::FromF32(srcPtr[0] * invNorm);
        }
    }
}
static void RGBA8888ToRG16F(const byte *src, byte *dst, int numPixels, bool isGamma) {
    const float invNorm = 1.0f / 255.0f;
    const byte *srcPtr = src;
    const byte *srcEnd = srcPtr + numPixels * 4;
    float16_t *dstPtr = (float16_t *)dst;

    if (isGamma) {
        for (; srcPtr < srcEnd; srcPtr += 4, dstPtr += 2) {
            dstPtr[0] = F16Converter::FromF32(gammaToLinearTable[srcPtr[0]]);
            dstPtr[1] = F16Converter::FromF32(gammaToLinearTable[srcPtr[1]]);
        }
    } else {
        for (; srcPtr < srcEnd; srcPtr += 4, dstPtr += 2) {
            dstPtr[0] = F16Converter::FromF32(srcPtr[0] * invNorm);
            dstPtr[1] = F16Converter::FromF32(srcPtr[1] * invNorm);
        }
    }
}
static void RGBA8888ToRGB16F(const byte *src, byte *dst, int numPixels, bool isGamma) {
    const float invNorm = 1.0f / 255.0f;
    const byte *srcPtr = src;
    const byte *srcEnd = srcPtr + numPixels * 4;
    float16_t *dstPtr = (float16_t *)dst;

    if (isGamma) {
        for (; srcPtr < srcEnd; srcPtr += 4, dstPtr += 3) {
            dstPtr[0] = F16Converter::FromF32(gammaToLinearTable[srcPtr[0]]);
            dstPtr[1] = F16Converter::FromF32(gammaToLinearTable[srcPtr[1]]);
            dstPtr[2] = F16Converter::FromF32(gammaToLinearTable[srcPtr[2]]);
        }
    } else {
        for (; srcPtr < srcEnd; srcPtr += 4, dstPtr += 3) {
            dstPtr[0] = F16Converter::FromF32(srcPtr[0] * invNorm);
            dstPtr[1] = F16Converter::FromF32(srcPtr[1] * invNorm);
            dstPtr[2] = F16Converter::FromF32(srcPtr[2] * invNorm);
        }
    }
}
static void RGBA8888ToRGBA16F(const byte *src, byte *dst, int numPixels, bool isGamma) {
    const float invNorm = 1.0f / 255.0f;
    const byte *srcPtr = src;
    const byte *srcEnd = srcPtr + numPixels * 4;
    float16_t *dstPtr = (float16_t *)dst;

    if (isGamma) {
        for (; srcPtr < srcEnd; srcPtr += 4, dstPtr += 3) {
            dstPtr[0] = F16Converter::FromF32(gammaToLinearTable[srcPtr[0]]);
            dstPtr[1] = F16Converter::FromF32(gammaToLinearTable[srcPtr[1]]);
            dstPtr[2] = F16Converter::FromF32(gammaToLinearTable[srcPtr[2]]);
            dstPtr[3] = F16Converter::FromF32(srcPtr[3] * invNorm);
        }
    } else {
        for (; srcPtr < srcEnd; srcPtr += 4, dstPtr += 3) {
            dstPtr[0] = F16Converter::FromF32(srcPtr[0] * invNorm);
            dstPtr[1] = F16Converter::FromF32(srcPtr[1] * invNorm);
            dstPtr[2] = F16Converter::FromF32(srcPtr[2] * invNorm);
            dstPtr[3] = F16Converter::FromF32(srcPtr[3] * invNorm);
        }
    }
}
static void RGBA8888ToL32F(const byte *src, byte *dst, int numPixels, bool isGamma) {
    const float invNorm = 1.0f / 255.0f;
    const byte *srcPtr = src;
    const byte *srcEnd = srcPtr + numPixels * 4;
    float *dstPtr = (float *)dst;

    if (isGamma) {
        for (; srcPtr < srcEnd; srcPtr += 4, dstPtr += 1) {
            dstPtr[0] = Image::GammaToLinearFast(0.299f * (srcPtr[0] * invNorm) + 0.587f * (srcPtr[1] * invNorm) + 0.114f * (srcPtr[2] * invNorm));
        }
    } else {
        for (; srcPtr < srcEnd; srcPtr += 4, dstPtr += 1) {
            dstPtr[0] = 0.299f * (srcPtr[0] * invNorm) + 0.587f * (srcPtr[1] * invNorm) + 0.114f * (srcPtr[2] * invNorm);
        }
    }
}
static void RGBA8888ToA32F(const byte *src, byte *dst, int numPixels, bool isGamma) {
    const float invNorm = 1.0f / 255.0f;
    const byte *srcPtr = src;
    const byte *srcEnd = srcPtr + numPixels * 4;
    float *dstPtr = (float *)dst;

    for (; srcPtr < srcEnd; srcPtr += 4, dstPtr += 1) {
        dstPtr[0] = srcPtr[3] * invNorm;
    }
}
static void RGBA8888ToLA32F(const byte *src, byte *dst, int numPixels, bool isGamma) {
    const float invNorm = 1.0f / 255.0f;
    const byte *srcPtr = src;
    const byte *srcEnd = srcPtr + numPixels * 4;
    float *dstPtr = (float *)dst;

    if (isGamma) {
        for (; srcPtr < srcEnd; srcPtr += 4, dstPtr += 2) {
            dstPtr[0] = Image::GammaToLinearFast(0.299f * (srcPtr[0] * invNorm) + 0.587f * (srcPtr[1] * invNorm) + 0.114f * (srcPtr[2] * invNorm));
            dstPtr[1] = srcPtr[3] * invNorm;
        }
    } else {
        for (; srcPtr < srcEnd; srcPtr += 4, dstPtr += 2) {
            dstPtr[0] = 0.299f * (srcPtr[0] * invNorm) + 0.587f * (srcPtr[1] * invNorm) + 0.114f * (srcPtr[2] * invNorm);
            dstPtr[1] = srcPtr[3] * invNorm;
        }
    }
}
static void RGBA8888ToR32F(const byte *src, byte *dst, int numPixels, bool isGamma) {
    const float invNorm = 1.0f / 255.0f;
    const byte *srcPtr = src;
    const byte *srcEnd = srcPtr + numPixels * 4;
    float *dstPtr = (float *)dst;

    if (isGamma) {
        for (; srcPtr < srcEnd; srcPtr += 4, dstPtr += 1) {
            dstPtr[0] = gammaToLinearTable[srcPtr[0]];
        }
    } else {
        for (; srcPtr < srcEnd; srcPtr += 4, dstPtr += 1) {
            dstPtr[0] = srcPtr[0] * invNorm;
        }
    }
}
static void RGBA8888ToRG32F(const byte *src, byte *dst, int numPixels, bool isGamma) {
    const float invNorm = 1.0f / 255.0f;
    const byte *srcPtr = src;
    const byte *srcEnd = srcPtr + numPixels * 4;
    float *dstPtr = (float *)dst;

    if (isGamma) {
        for (; srcPtr < srcEnd; srcPtr += 4, dstPtr += 2) {
            dstPtr[0] = gammaToLinearTable[srcPtr[0]];
            dstPtr[1] = gammaToLinearTable[srcPtr[1]];
        }
    } else {
        for (; srcPtr < srcEnd; srcPtr += 4, dstPtr += 2) {
            dstPtr[0] = srcPtr[0] * invNorm;
            dstPtr[1] = srcPtr[1] * invNorm;
        }
    }
}
static void RGBA8888ToRGB32F(const byte *src, byte *dst, int numPixels, bool isGamma) {
    const float invNorm = 1.0f / 255.0f;
    const byte *srcPtr = src;
    const byte *srcEnd = srcPtr + numPixels * 4;
    float *dstPtr = (float *)dst;

    if (isGamma) {
        for (; srcPtr < srcEnd; srcPtr += 4, dstPtr += 3) {
            dstPtr[0] = gammaToLinearTable[srcPtr[0]];
            dstPtr[1] = gammaToLinearTable[srcPtr[1]];
            dstPtr[2] = gammaToLinearTable[srcPtr[2]];
        }
    } else {
        for (; srcPtr < srcEnd; srcPtr += 4, dstPtr += 3) {
            dstPtr[0] = srcPtr[0] * invNorm;
            dstPtr[1] = srcPtr[1] * invNorm;
            dstPtr[2] = srcPtr[2] * invNorm;
        }
    }
}
static void RGBA8888ToRGBA32F(const byte *src, byte *dst, int numPixels, bool isGamma) {
    const float invNorm = 1.0f / 255.0f;
    const byte *srcPtr = src;
    const byte *srcEnd = srcPtr + numPixels * 4;
    float *dstPtr = (float *)dst;

    if (isGamma) {
        for (; srcPtr < srcEnd; srcPtr += 4, dstPtr += 3) {
            dstPtr[0] = gammaToLinearTable[srcPtr[0]];
            dstPtr[1] = gammaToLinearTable[srcPtr[1]];
            dstPtr[2] = gammaToLinearTable[srcPtr[2]];
            dstPtr[3] = srcPtr[3] * invNorm;
        }
    } else {
        for (; srcPtr < srcEnd; srcPtr += 4, dstPtr += 3) {
            dstPtr[0] = srcPtr[0] * invNorm;
            dstPtr[1] = srcPtr[1] * invNorm;
            dstPtr[2] = srcPtr[2] * invNorm;
            dstPtr[3] = srcPtr[3] * invNorm;
        }
    }
}
static void RGBA8888ToRGB11F11F10F(const byte *src, byte *dst, int numPixels, bool isGamma) {
    const float invNorm = 1.0f / 255.0f;
    const byte *srcPtr = src;
    const byte *srcEnd = srcPtr + numPixels * 4;
    uint32_t *dstPtr = (uint32_t *)dst;

    if (isGamma) {
        for (; srcPtr < srcEnd; srcPtr += 4, dstPtr += 1) {
            uint32_t r = F11Converter::FromF32(gammaToLinearTable[srcPtr[0]]);
            uint32_t g = F11Converter::FromF32(gammaToLinearTable[srcPtr[1]]);
            uint32_t b = F10Converter::FromF32(gammaToLinearTable[srcPtr[2]]);
            *dstPtr = r | (g << 11) | (b << 22);
        }
    } else {
        for (; srcPtr < srcEnd; srcPtr += 4, dstPtr += 1) {
            uint32_t r = F11Converter::FromF32(srcPtr[0] * invNorm);
            uint32_t g = F11Converter::FromF32(srcPtr[1] * invNorm);
            uint32_t b = F10Converter::FromF32(srcPtr[2] * invNorm);
            *dstPtr = r | (g << 11) | (b << 22);
        }
    }
}

//--------------------------------------------------------------------------------------------------
//
// XXXToRGBA32F (unpacking function from custom format to rgba32f)
//
//--------------------------------------------------------------------------------------------------

static void L8ToRGBA32F(const byte *src, byte *dst, int numPixels, bool isGamma) {
    const byte *srcPtr = src;
    const byte *srcEnd = srcPtr + numPixels;
    float *dstPtr = (float *)dst;

    if (isGamma) {
        for (; srcPtr < srcEnd; srcPtr += 1, dstPtr += 4) {
            dstPtr[0] = dstPtr[1] = dstPtr[2] = gammaToLinearTable[srcPtr[0]];
            dstPtr[3] = 1.0f;
        }
    } else {
        for (; srcPtr < srcEnd; srcPtr += 1, dstPtr += 4) {
            dstPtr[0] = dstPtr[1] = dstPtr[2] = srcPtr[0] / 255.0f;
            dstPtr[3] = 1.0f;
        }
    }
}
static void A8ToRGBA32F(const byte *src, byte *dst, int numPixels, bool isGamma) {
    const byte *srcPtr = src;
    const byte *srcEnd = srcPtr + numPixels;
    float *dstPtr = (float *)dst;

    for (; srcPtr < srcEnd; srcPtr += 1, dstPtr += 4) {
        dstPtr[0] = dstPtr[1] = dstPtr[2] = 1.0f;
        dstPtr[3] = srcPtr[0] / 255.0f;
    }
}
static void LA88ToRGBA32F(const byte *src, byte *dst, int numPixels, bool isGamma) {
    const byte *srcPtr = src;
    const byte *srcEnd = srcPtr + numPixels * 2;
    float *dstPtr = (float *)dst;

    if (isGamma) {
        for (; srcPtr < srcEnd; srcPtr += 2, dstPtr += 4) {
            dstPtr[0] = dstPtr[1] = dstPtr[2] = gammaToLinearTable[srcPtr[0]];
            dstPtr[3] = srcPtr[1] / 255.0f;
        }
    } else {
        for (; srcPtr < srcEnd; srcPtr += 2, dstPtr += 4) {
            dstPtr[0] = dstPtr[1] = dstPtr[2] = srcPtr[0] / 255.0f;
            dstPtr[3] = srcPtr[1] / 255.0f;
        }
    }
}
static void LA1616ToRGBA32F(const byte *src, byte *dst, int numPixels, bool isGamma) {
    const uint16_t *srcPtr = (const uint16_t *)src;
    const uint16_t *srcEnd = srcPtr + numPixels * 2;
    float *dstPtr = (float *)dst;
    float l, a;

    if (isGamma) {
        for (; srcPtr < srcEnd; srcPtr += 2, dstPtr += 4) {
            l = Image::GammaToLinearFast(srcPtr[0] / 65535.0f);
            a = srcPtr[1] / 65535.0f;
            dstPtr[0] = l;
            dstPtr[1] = l;
            dstPtr[2] = l;
            dstPtr[3] = a;
        }
    } else {
        for (; srcPtr < srcEnd; srcPtr += 2, dstPtr += 4) {
            l = srcPtr[0] / 65535.0f;
            a = srcPtr[1] / 65535.0f;
            dstPtr[0] = l;
            dstPtr[1] = l;
            dstPtr[2] = l;
            dstPtr[3] = a;
        }
    }
}
static void R8ToRGBA32F(const byte *src, byte *dst, int numPixels, bool isGamma) {
    const byte *srcPtr = src;
    const byte *srcEnd = srcPtr + numPixels;
    float *dstPtr = (float *)dst;

    if (isGamma) {
        for (; srcPtr < srcEnd; srcPtr += 1, dstPtr += 4) {
            dstPtr[0] = gammaToLinearTable[srcPtr[0]];
            dstPtr[1] = 0;
            dstPtr[2] = 0;
            dstPtr[3] = 1.0f;
        }
    } else {
        for (; srcPtr < srcEnd; srcPtr += 1, dstPtr += 4) {
            dstPtr[0] = srcPtr[0] / 255.0f;
            dstPtr[1] = 0;
            dstPtr[2] = 0;
            dstPtr[3] = 1.0f;
        }
    }
}
static void RG88ToRGBA32F(const byte *src, byte *dst, int numPixels, bool isGamma) {
    const byte *srcPtr = src;
    const byte *srcEnd = srcPtr + numPixels * 2;
    float *dstPtr = (float *)dst;

    if (isGamma) {
        for (; srcPtr < srcEnd; srcPtr += 2, dstPtr += 4) {
            dstPtr[0] = gammaToLinearTable[srcPtr[0]];
            dstPtr[1] = gammaToLinearTable[srcPtr[1]];
            dstPtr[2] = 0;
            dstPtr[3] = 1.0f;
        }
    } else {
        for (; srcPtr < srcEnd; srcPtr += 2, dstPtr += 4) {
            dstPtr[0] = srcPtr[0] / 255.0f;
            dstPtr[1] = srcPtr[1] / 255.0f;
            dstPtr[2] = 0;
            dstPtr[3] = 1.0f;
        }
    }
}
static void RGB888ToRGBA32F(const byte *src, byte *dst, int numPixels, bool isGamma) {
    const byte *srcPtr = src;
    const byte *srcEnd = srcPtr + numPixels * 3;
    float *dstPtr = (float *)dst;

    if (isGamma) {
        for (; srcPtr < srcEnd; srcPtr += 3, dstPtr += 4) {
            dstPtr[0] = gammaToLinearTable[srcPtr[0]];
            dstPtr[1] = gammaToLinearTable[srcPtr[1]];
            dstPtr[2] = gammaToLinearTable[srcPtr[2]];
            dstPtr[3] = 1.0f;
        }
    } else {
        for (; srcPtr < srcEnd; srcPtr += 3, dstPtr += 4) {
            dstPtr[0] = srcPtr[0] / 255.0f;
            dstPtr[1] = srcPtr[1] / 255.0f;
            dstPtr[2] = srcPtr[2] / 255.0f;
            dstPtr[3] = 1.0f;
        }
    }
}
static void BGR888ToRGBA32F(const byte *src, byte *dst, int numPixels, bool isGamma) {
    const byte *srcPtr = src;
    const byte *srcEnd = srcPtr + numPixels * 3;
    float *dstPtr = (float *)dst;

    if (isGamma) {
        for (; srcPtr < srcEnd; srcPtr += 3, dstPtr += 4) {
            dstPtr[0] = gammaToLinearTable[srcPtr[2]];
            dstPtr[1] = gammaToLinearTable[srcPtr[1]];
            dstPtr[2] = gammaToLinearTable[srcPtr[0]];
            dstPtr[3] = 1.0f;
        }
    } else {
        for (; srcPtr < srcEnd; srcPtr += 3, dstPtr += 4) {
            dstPtr[0] = srcPtr[2] / 255.0f;
            dstPtr[1] = srcPtr[1] / 255.0f;
            dstPtr[2] = srcPtr[0] / 255.0f;
            dstPtr[3] = 1.0f;
        }
    }
}
static void RGBX8888ToRGBA32F(const byte *src, byte *dst, int numPixels, bool isGamma) {
    const byte *srcPtr = src;
    const byte *srcEnd = srcPtr + numPixels * 4;
    float *dstPtr = (float *)dst;

    if (isGamma) {
        for (; srcPtr < srcEnd; srcPtr += 4, dstPtr += 4) {
            dstPtr[0] = gammaToLinearTable[srcPtr[0]];
            dstPtr[1] = gammaToLinearTable[srcPtr[1]];
            dstPtr[2] = gammaToLinearTable[srcPtr[2]];
            dstPtr[3] = 1.0f;
        }
    } else {
        for (; srcPtr < srcEnd; srcPtr += 4, dstPtr += 4) {
            dstPtr[0] = srcPtr[0] / 255.0f;
            dstPtr[1] = srcPtr[1] / 255.0f;
            dstPtr[2] = srcPtr[2] / 255.0f;
            dstPtr[3] = 1.0f;
        }
    }
}
static void BGRX8888ToRGBA32F(const byte *src, byte *dst, int numPixels, bool isGamma) {
    const byte *srcPtr = src;
    const byte *srcEnd = srcPtr + numPixels * 4;
    float *dstPtr = (float *)dst;

    if (isGamma) {
        for (; srcPtr < srcEnd; srcPtr += 4, dstPtr += 4) {
            dstPtr[0] = gammaToLinearTable[srcPtr[2]];
            dstPtr[1] = gammaToLinearTable[srcPtr[1]];
            dstPtr[2] = gammaToLinearTable[srcPtr[0]];
            dstPtr[3] = 1.0f;
        }
    } else {
        for (; srcPtr < srcEnd; srcPtr += 4, dstPtr += 4) {
            dstPtr[0] = srcPtr[2] / 255.0f;
            dstPtr[1] = srcPtr[1] / 255.0f;
            dstPtr[2] = srcPtr[0] / 255.0f;
            dstPtr[3] = 1.0f;
        }
    }
}
static void BGRA8888ToRGBA32F(const byte *src, byte *dst, int numPixels, bool isGamma) {
    const byte *srcPtr = src;
    const byte *srcEnd = srcPtr + numPixels * 4;
    float *dstPtr = (float *)dst;

    if (isGamma) {
        for (; srcPtr < srcEnd; srcPtr += 4, dstPtr += 4) {
            dstPtr[0] = gammaToLinearTable[srcPtr[2]];
            dstPtr[1] = gammaToLinearTable[srcPtr[1]];
            dstPtr[2] = gammaToLinearTable[srcPtr[0]];
            dstPtr[3] = srcPtr[3] / 255.0f;
        }
    } else {
        for (; srcPtr < srcEnd; srcPtr += 4, dstPtr += 4) {
            dstPtr[0] = srcPtr[2] / 255.0f;
            dstPtr[1] = srcPtr[1] / 255.0f;
            dstPtr[2] = srcPtr[0] / 255.0f;
            dstPtr[3] = srcPtr[3] / 255.0f;
        }
    }
}
static void ABGR8888ToRGBA32F(const byte *src, byte *dst, int numPixels, bool isGamma) {
    const byte *srcPtr = src;
    const byte *srcEnd = srcPtr + numPixels * 4;
    float *dstPtr = (float *)dst;

    if (isGamma) {
        for (; srcPtr < srcEnd; srcPtr += 4, dstPtr += 4) {
            dstPtr[0] = gammaToLinearTable[srcPtr[3]];
            dstPtr[1] = gammaToLinearTable[srcPtr[2]];
            dstPtr[2] = gammaToLinearTable[srcPtr[1]];
            dstPtr[3] = srcPtr[0] / 255.0f;
        }
    } else {
        for (; srcPtr < srcEnd; srcPtr += 4, dstPtr += 4) {
            dstPtr[0] = srcPtr[3] / 255.0f;
            dstPtr[1] = srcPtr[2] / 255.0f;
            dstPtr[2] = srcPtr[1] / 255.0f;
            dstPtr[3] = srcPtr[0] / 255.0f;
        }
    }
}
static void ARGB8888ToRGBA32F(const byte *src, byte *dst, int numPixels, bool isGamma) {
    const byte *srcPtr = src;
    const byte *srcEnd = srcPtr + numPixels * 4;
    float *dstPtr = (float *)dst;

    if (isGamma) {
        for (; srcPtr < srcEnd; srcPtr += 4, dstPtr += 4) {
            dstPtr[0] = gammaToLinearTable[srcPtr[1]];
            dstPtr[1] = gammaToLinearTable[srcPtr[2]];
            dstPtr[2] = gammaToLinearTable[srcPtr[3]];
            dstPtr[3] = srcPtr[0] / 255.0f;
        }
    } else {
        for (; srcPtr < srcEnd; srcPtr += 4, dstPtr += 4) {
            dstPtr[0] = srcPtr[1] / 255.0f;
            dstPtr[1] = srcPtr[2] / 255.0f;
            dstPtr[2] = srcPtr[3] / 255.0f;
            dstPtr[3] = srcPtr[0] / 255.0f;
        }
    }
}
static void RGBE9995ToRGBA32F(const byte *src, byte *dst, int numPixels, bool isGamma) {
    const uint32_t *srcPtr = (const uint32_t *)src;
    const uint32_t *srcEnd = srcPtr + numPixels;
    byte *dstPtr = dst;
    float m;

    for (; srcPtr < srcEnd; srcPtr++, dstPtr += 4) {
        m = Math::Pow(2, ((*srcPtr >> 27) & 0x1F) - 24);
        dstPtr[0] = (*srcPtr & 0x1FF) * m;
        dstPtr[1] = ((*srcPtr >> 9) & 0x1FF) * m;
        dstPtr[2] = ((*srcPtr >> 18) & 0x1FF) * m;
        dstPtr[3] = 255;
    }
}
static void L16FToRGBA32F(const byte *src, byte *dst, int numPixels, bool isGamma) {
    const float16_t *srcPtr = (const float16_t *)src;
    const float16_t *srcEnd = srcPtr + numPixels;
    float *dstPtr = (float *)dst;

    for (; srcPtr < srcEnd; srcPtr += 1, dstPtr += 4) {
        dstPtr[0] = dstPtr[1] = dstPtr[2] = F16Converter::ToF32(srcPtr[0]);
        dstPtr[3] = 1.0f;
    }
}
static void A16FToRGBA32F(const byte *src, byte *dst, int numPixels, bool isGamma) {
    const float16_t *srcPtr = (const float16_t *)src;
    const float16_t *srcEnd = srcPtr + numPixels;
    float *dstPtr = (float *)dst;

    for (; srcPtr < srcEnd; srcPtr += 1, dstPtr += 4) {
        dstPtr[0] = 1.0f;
        dstPtr[1] = 1.0f;
        dstPtr[2] = 1.0f;
        dstPtr[3] = F16Converter::ToF32(srcPtr[0]);
    }
}
static void LA16FToRGBA32F(const byte *src, byte *dst, int numPixels, bool isGamma) {
    const float16_t *srcPtr = (const float16_t *)src;
    const float16_t *srcEnd = srcPtr + numPixels * 2;
    float *dstPtr = (float *)dst;

    for (; srcPtr < srcEnd; srcPtr += 2, dstPtr += 4) {
        dstPtr[0] = dstPtr[1] = dstPtr[2] = F16Converter::ToF32(srcPtr[0]);
        dstPtr[3] = F16Converter::ToF32(srcPtr[1]);
    }
}
static void R16FToRGBA32F(const byte *src, byte *dst, int numPixels, bool isGamma) {
    const float16_t *srcPtr = (const float16_t *)src;
    const float16_t *srcEnd = srcPtr + numPixels;
    float *dstPtr = (float *)dst;

    for (; srcPtr < srcEnd; srcPtr += 1, dstPtr += 4) {
        dstPtr[0] = F16Converter::ToF32(srcPtr[0]);
        dstPtr[1] = 0;
        dstPtr[2] = 0;
        dstPtr[3] = 1.0f;
    }
}
static void RG16FToRGBA32F(const byte *src, byte *dst, int numPixels, bool isGamma) {
    const float16_t *srcPtr = (const float16_t *)src;
    const float16_t *srcEnd = srcPtr + numPixels * 2;
    float *dstPtr = (float *)dst;

    for (; srcPtr < srcEnd; srcPtr += 2, dstPtr += 4) {
        dstPtr[0] = F16Converter::ToF32(srcPtr[0]);
        dstPtr[1] = F16Converter::ToF32(srcPtr[1]);
        dstPtr[2] = 0;
        dstPtr[3] = 1.0f;
    }
}
static void RGB16FToRGBA32F(const byte *src, byte *dst, int numPixels, bool isGamma) {
    const float16_t *srcPtr = (const float16_t *)src;
    const float16_t *srcEnd = srcPtr + numPixels * 3;
    float *dstPtr = (float *)dst;

    for (; srcPtr < srcEnd; srcPtr += 3, dstPtr += 4) {
        dstPtr[0] = F16Converter::ToF32(srcPtr[0]);
        dstPtr[1] = F16Converter::ToF32(srcPtr[1]);
        dstPtr[2] = F16Converter::ToF32(srcPtr[2]);
        dstPtr[3] = 1.0f;
    }
}
static void RGBA16FToRGBA32F(const byte *src, byte *dst, int numPixels, bool isGamma) {
    const float16_t *srcPtr = (const float16_t *)src;
    const float16_t *srcEnd = srcPtr + numPixels * 4;
    float *dstPtr = (float *)dst;

    for (; srcPtr < srcEnd; srcPtr += 4, dstPtr += 4) {
        dstPtr[0] = F16Converter::ToF32(srcPtr[0]);
        dstPtr[1] = F16Converter::ToF32(srcPtr[1]);
        dstPtr[2] = F16Converter::ToF32(srcPtr[2]);
        dstPtr[3] = F16Converter::ToF32(srcPtr[3]);
    }
}
static void L32FToRGBA32F(const byte *src, byte *dst, int numPixels, bool isGamma) {
    const float *srcPtr = (const float *)src;
    const float *srcEnd = srcPtr + numPixels;
    float *dstPtr = (float *)dst;

    for (; srcPtr < srcEnd; srcPtr += 1, dstPtr += 4) {
        dstPtr[0] = dstPtr[1] = dstPtr[2] = srcPtr[0];
        dstPtr[3] = 1.0f;
    }
}

static void A32FToRGBA32F(const byte *src, byte *dst, int numPixels, bool isGamma) {
    const float *srcPtr = (const float *)src;
    const float *srcEnd = srcPtr + numPixels;
    float *dstPtr = (float *)dst;

    for (; srcPtr < srcEnd; srcPtr += 1, dstPtr += 4) {
        dstPtr[0] = 1.0f;
        dstPtr[1] = 1.0f;
        dstPtr[2] = 1.0f;
        dstPtr[3] = srcPtr[0];
    }
}
static void LA32FToRGBA32F(const byte *src, byte *dst, int numPixels, bool isGamma) {
    const float *srcPtr = (const float *)src;
    const float *srcEnd = srcPtr + numPixels * 2;
    float *dstPtr = (float *)dst;

    for (; srcPtr < srcEnd; srcPtr += 2, dstPtr += 4) {
        dstPtr[0] = dstPtr[1] = dstPtr[2] = srcPtr[0];
        dstPtr[3] = srcPtr[1];
    }
}
static void R32FToRGBA32F(const byte *src, byte *dst, int numPixels, bool isGamma) {
    const float *srcPtr = (const float *)src;
    const float *srcEnd = srcPtr + numPixels;
    float *dstPtr = (float *)dst;

    for (; srcPtr < srcEnd; srcPtr += 1, dstPtr += 4) {
        dstPtr[0] = srcPtr[0];
        dstPtr[1] = 0;
        dstPtr[2] = 0;
        dstPtr[3] = 1.0f;
    }
}
static void RG32FToRGBA32F(const byte *src, byte *dst, int numPixels, bool isGamma) {
    const float *srcPtr = (const float *)src;
    const float *srcEnd = srcPtr + numPixels * 2;
    float *dstPtr = (float *)dst;

    for (; srcPtr < srcEnd; srcPtr += 2, dstPtr += 4) {
        dstPtr[0] = srcPtr[0];
        dstPtr[1] = srcPtr[1];
        dstPtr[2] = 0;
        dstPtr[3] = 1.0f;
    }
}
static void RGB32FToRGBA32F(const byte *src, byte *dst, int numPixels, bool isGamma) {
    const float *srcPtr = (const float *)src;
    const float *srcEnd = srcPtr + numPixels * 3;
    float *dstPtr = (float *)dst;

    for (; srcPtr < srcEnd; srcPtr += 3, dstPtr += 4) {
        dstPtr[0] = srcPtr[0];
        dstPtr[1] = srcPtr[1];
        dstPtr[2] = srcPtr[2];
        dstPtr[3] = 1.0f;
    }
}
static void RGBA32FToRGBA32F(const byte *src, byte *dst, int numPixels, bool isGamma) {
    memcpy(dst, src, sizeof(float) * 4 * numPixels);
}
static void RGB11F11F10FToRGBA32F(const byte *src, byte *dst, int numPixels, bool isGamma) {
    const uint32_t *srcPtr = (const uint32_t *)src;
    const uint32_t *srcEnd = srcPtr + numPixels;
    float *dstPtr = (float *)dst;

    for (; srcPtr < srcEnd; srcPtr += 1, dstPtr += 4) {
        dstPtr[0] = F11Converter::ToF32(*srcPtr & 0x7FF);
        dstPtr[1] = F11Converter::ToF32((*srcPtr >> 11) & 0x7FF);
        dstPtr[2] = F10Converter::ToF32((*srcPtr >> 22) & 0x3FF);
        dstPtr[3] = 1.0f;
    }
}

//--------------------------------------------------------------------------------------------------
//
// RGBA32FToXXX (packing function from rgba32f to custom format)
//
//--------------------------------------------------------------------------------------------------

static void RGBA32FToL8(const byte *src, byte *dst, int numPixels, bool isGamma) {
    const float *srcPtr = (const float *)src;
    const float *srcEnd = srcPtr + numPixels * 4;
    byte *dstPtr = dst;

    for (; srcPtr < srcEnd; srcPtr += 4, dstPtr += 1) {
        dstPtr[0] = Math::Ftob(255.0f * (0.299f * srcPtr[0] + 0.587f * srcPtr[1] + 0.114f * srcPtr[2]));
    }
}
static void RGBA32FToA8(const byte *src, byte *dst, int numPixels, bool isGamma) {
    const float *srcPtr = (const float *)src;
    const float *srcEnd = srcPtr + numPixels * 4;
    byte *dstPtr = dst;

    for (; srcPtr < srcEnd; srcPtr += 4, dstPtr += 1) {
        dstPtr[0] = Math::Ftob(255.0f * srcPtr[3]);
    }
}
static void RGBA32FToLA88(const byte *src, byte *dst, int numPixels, bool isGamma) {
    const float *srcPtr = (const float *)src;
    const float *srcEnd = srcPtr + numPixels * 4;
    byte *dstPtr = dst;

    for (; srcPtr < srcEnd; srcPtr += 4, dstPtr += 2) {
        dstPtr[0] = Math::Ftob(255.0f * (0.299f * srcPtr[0] + 0.587f * srcPtr[1] + 0.114f * srcPtr[2]));
        dstPtr[1] = Math::Ftob(255.0f * srcPtr[3]);
    }
}
static void RGBA32FToLA1616(const byte *src, byte *dst, int numPixels, bool isGamma) {
    const float *srcPtr = (const float *)src;
    const float *srcEnd = srcPtr + numPixels * 4;
    uint16_t *dstPtr = (uint16_t *)dst;

    for (; srcPtr < srcEnd; srcPtr += 4, dstPtr += 2) {
        dstPtr[0] = Clamp<int>((0.299f * srcPtr[0] + 0.587f * srcPtr[1] + 0.114f * srcPtr[2]) * 65535.0f, 0, 65535);
        dstPtr[1] = Clamp<int>(srcPtr[3] * 65535.0f, 0, 65535);
    }
}
static void RGBA32FToR8(const byte *src, byte *dst, int numPixels, bool isGamma) {
    const float *srcPtr = (const float *)src;
    const float *srcEnd = srcPtr + numPixels * 4;
    byte *dstPtr = dst;

    for (; srcPtr < srcEnd; srcPtr += 4, dstPtr += 1) {
        dstPtr[0] = Math::Ftob(255.0f * srcPtr[0]);
    }
}
static void RGBA32FToRG88(const byte *src, byte *dst, int numPixels, bool isGamma) {
    const float *srcPtr = (const float *)src;
    const float *srcEnd = srcPtr + numPixels * 4;
    byte *dstPtr = dst;

    for (; srcPtr < srcEnd; srcPtr += 4, dstPtr += 2) {
        dstPtr[0] = Math::Ftob(255.0f * srcPtr[0]);
        dstPtr[1] = Math::Ftob(255.0f * srcPtr[1]);
    }
}
static void RGBA32FToRGB888(const byte *src, byte *dst, int numPixels, bool isGamma) {
    const float *srcPtr = (const float *)src;
    const float *srcEnd = srcPtr + numPixels * 4;
    byte *dstPtr = dst;

    for (; srcPtr < srcEnd; srcPtr += 4, dstPtr += 3) {
        dstPtr[0] = Math::Ftob(255.0f * srcPtr[0]);
        dstPtr[1] = Math::Ftob(255.0f * srcPtr[1]);
        dstPtr[2] = Math::Ftob(255.0f * srcPtr[2]);
    }
}
static void RGBA32FToBGR888(const byte *src, byte *dst, int numPixels, bool isGamma) {
    const float *srcPtr = (const float *)src;
    const float *srcEnd = srcPtr + numPixels * 4;
    byte *dstPtr = dst;

    for (; srcPtr < srcEnd; srcPtr += 4, dstPtr += 3) {
        dstPtr[0] = Math::Ftob(255.0f * srcPtr[2]);
        dstPtr[1] = Math::Ftob(255.0f * srcPtr[1]);
        dstPtr[2] = Math::Ftob(255.0f * srcPtr[0]);
    }
}
static void RGBA32FToRGBX8888(const byte *src, byte *dst, int numPixels, bool isGamma) {
    const float *srcPtr = (const float *)src;
    const float *srcEnd = srcPtr + numPixels * 4;
    byte *dstPtr = dst;

    for (; srcPtr < srcEnd; srcPtr += 4, dstPtr += 4) {
        dstPtr[0] = Math::Ftob(255.0f * srcPtr[0]);
        dstPtr[1] = Math::Ftob(255.0f * srcPtr[1]);
        dstPtr[2] = Math::Ftob(255.0f * srcPtr[2]);
        dstPtr[3] = 255;
    }
}
static void RGBA32FToBGRX8888(const byte *src, byte *dst, int numPixels, bool isGamma) {
    const float *srcPtr = (const float *)src;
    const float *srcEnd = srcPtr + numPixels * 4;
    byte *dstPtr = dst;

    for (; srcPtr < srcEnd; srcPtr += 4, dstPtr += 4) {
        dstPtr[0] = Math::Ftob(255.0f * srcPtr[2]);
        dstPtr[1] = Math::Ftob(255.0f * srcPtr[1]);
        dstPtr[2] = Math::Ftob(255.0f * srcPtr[0]);
        dstPtr[3] = 255;
    }
}
static void RGBA32FToBGRA8888(const byte *src, byte *dst, int numPixels, bool isGamma) {
    const float *srcPtr = (const float *)src;
    const float *srcEnd = srcPtr + numPixels * 4;
    byte *dstPtr = dst;

    if (isGamma) {
        for (; srcPtr < srcEnd; srcPtr += 4, dstPtr += 4) {
            dstPtr[0] = Math::Ftob(255.0f * Image::LinearToGammaFast(srcPtr[2]));
            dstPtr[1] = Math::Ftob(255.0f * Image::LinearToGammaFast(srcPtr[1]));
            dstPtr[2] = Math::Ftob(255.0f * Image::LinearToGammaFast(srcPtr[0]));
            dstPtr[3] = Math::Ftob(255.0f * srcPtr[3]);
        }
    } else {
        for (; srcPtr < srcEnd; srcPtr += 4, dstPtr += 4) {
            dstPtr[0] = Math::Ftob(255.0f * srcPtr[2]);
            dstPtr[1] = Math::Ftob(255.0f * srcPtr[1]);
            dstPtr[2] = Math::Ftob(255.0f * srcPtr[0]);
            dstPtr[3] = Math::Ftob(255.0f * srcPtr[3]);
        }
    }
}
static void RGBA32FToABGR8888(const byte *src, byte *dst, int numPixels, bool isGamma) {
    const float *srcPtr = (const float *)src;
    const float *srcEnd = srcPtr + numPixels * 4;
    byte *dstPtr = dst;

    if (isGamma) {
        for (; srcPtr < srcEnd; srcPtr += 4, dstPtr += 4) {
            dstPtr[0] = Math::Ftob(255.0f * srcPtr[3]);
            dstPtr[1] = Math::Ftob(255.0f * Image::LinearToGammaFast(srcPtr[2]));
            dstPtr[2] = Math::Ftob(255.0f * Image::LinearToGammaFast(srcPtr[1]));
            dstPtr[3] = Math::Ftob(255.0f * Image::LinearToGammaFast(srcPtr[0]));
        }
    } else {
        for (; srcPtr < srcEnd; srcPtr += 4, dstPtr += 4) {
            dstPtr[0] = Math::Ftob(255.0f * srcPtr[3]);
            dstPtr[1] = Math::Ftob(255.0f * srcPtr[2]);
            dstPtr[2] = Math::Ftob(255.0f * srcPtr[1]);
            dstPtr[3] = Math::Ftob(255.0f * srcPtr[0]);
        }
    }
}
static void RGBA32FToARGB8888(const byte *src, byte *dst, int numPixels, bool isGamma) {
    const float *srcPtr = (const float *)src;
    const float *srcEnd = srcPtr + numPixels * 4;
    byte *dstPtr = dst;

    if (isGamma) {
        for (; srcPtr < srcEnd; srcPtr += 4, dstPtr += 4) {
            dstPtr[0] = Math::Ftob(255.0f * srcPtr[3]);
            dstPtr[1] = Math::Ftob(255.0f * Image::LinearToGammaFast(srcPtr[0]));
            dstPtr[2] = Math::Ftob(255.0f * Image::LinearToGammaFast(srcPtr[1]));
            dstPtr[3] = Math::Ftob(255.0f * Image::LinearToGammaFast(srcPtr[2]));
        }
    } else {
        for (; srcPtr < srcEnd; srcPtr += 4, dstPtr += 4) {
            dstPtr[0] = Math::Ftob(255.0f * srcPtr[3]);
            dstPtr[1] = Math::Ftob(255.0f * srcPtr[0]);
            dstPtr[2] = Math::Ftob(255.0f * srcPtr[1]);
            dstPtr[3] = Math::Ftob(255.0f * srcPtr[2]);
        }
    }
}
static void RGBA32FToRGBE9995(const byte *src, byte *dst, int numPixels, bool isGamma) {
    const float *srcPtr = (const float *)src;
    const float *srcEnd = srcPtr + numPixels * 4;
    uint32_t *dstPtr = (uint32_t *)dst;

    for (; srcPtr < srcEnd; srcPtr += 4, dstPtr++) {
        *dstPtr = RGBE9995::FromColor3(srcPtr[0], srcPtr[1], srcPtr[1]);
    }
}
static void RGBA32FToL16F(const byte *src, byte *dst, int numPixels, bool isGamma) {
    const float *srcPtr = (const float *)src;
    const float *srcEnd = srcPtr + numPixels * 4;
    float16_t *dstPtr = (float16_t *)dst;

    for (; srcPtr < srcEnd; srcPtr += 4, dstPtr += 1) {
        dstPtr[0] = F16Converter::FromF32(0.299f * srcPtr[0] + 0.587f * srcPtr[1] + 0.114f * srcPtr[2]);
    }
}
static void RGBA32FToA16F(const byte *src, byte *dst, int numPixels, bool isGamma) {
    const float *srcPtr = (const float *)src;
    const float *srcEnd = srcPtr + numPixels * 4;
    float16_t *dstPtr = (float16_t *)dst;

    for (; srcPtr < srcEnd; srcPtr += 4, dstPtr += 1) {
        dstPtr[0] = F16Converter::FromF32(srcPtr[3]);
    }
}
static void RGBA32FToLA16F(const byte *src, byte *dst, int numPixels, bool isGamma) {
    const float *srcPtr = (const float *)src;
    const float *srcEnd = srcPtr + numPixels * 4;
    float16_t *dstPtr = (float16_t *)dst;

    for (; srcPtr < srcEnd; srcPtr += 4, dstPtr += 2) {
        dstPtr[0] = F16Converter::FromF32(0.299f * srcPtr[0] + 0.587f * srcPtr[1] + 0.114f * srcPtr[2]);
        dstPtr[1] = F16Converter::FromF32(srcPtr[3]);
    }
}
static void RGBA32FToR16F(const byte *src, byte *dst, int numPixels, bool isGamma) {
    const float *srcPtr = (const float *)src;
    const float *srcEnd = srcPtr + numPixels * 4;
    float16_t *dstPtr = (float16_t *)dst;

    for (; srcPtr < srcEnd; srcPtr += 4, dstPtr += 1) {
        dstPtr[0] = F16Converter::FromF32(srcPtr[0]);
    }
}
static void RGBA32FToRG16F(const byte *src, byte *dst, int numPixels, bool isGamma) {
    const float *srcPtr = (const float *)src;
    const float *srcEnd = srcPtr + numPixels * 4;
    float16_t *dstPtr = (float16_t *)dst;

    for (; srcPtr < srcEnd; srcPtr += 4, dstPtr += 2) {
        dstPtr[0] = F16Converter::FromF32(srcPtr[0]);
        dstPtr[1] = F16Converter::FromF32(srcPtr[1]);
    }
}
static void RGBA32FToRGB16F(const byte *src, byte *dst, int numPixels, bool isGamma) {
    const float *srcPtr = (const float *)src;
    const float *srcEnd = srcPtr + numPixels * 4;
    float16_t *dstPtr = (float16_t *)dst;

    for (; srcPtr < srcEnd; srcPtr += 4, dstPtr += 3) {
        dstPtr[0] = F16Converter::FromF32(srcPtr[0]);
        dstPtr[1] = F16Converter::FromF32(srcPtr[1]);
        dstPtr[2] = F16Converter::FromF32(srcPtr[2]);
    }
}
static void RGBA32FToRGBA16F(const byte *src, byte *dst, int numPixels, bool isGamma) {
    const float *srcPtr = (const float *)src;
    const float *srcEnd = srcPtr + numPixels * 4;
    float16_t *dstPtr = (float16_t *)dst;

    for (; srcPtr < srcEnd; srcPtr += 4, dstPtr += 4) {
        dstPtr[0] = F16Converter::FromF32(srcPtr[0]);
        dstPtr[1] = F16Converter::FromF32(srcPtr[1]);
        dstPtr[2] = F16Converter::FromF32(srcPtr[2]);
        dstPtr[3] = F16Converter::FromF32(srcPtr[3]);
    }
}
static void RGBA32FToL32F(const byte *src, byte *dst, int numPixels, bool isGamma) {
    const float *srcPtr = (const float *)src;
    const float *srcEnd = srcPtr + numPixels * 4;
    float *dstPtr = (float *)dst;

    for (; srcPtr < srcEnd; srcPtr += 4, dstPtr += 1) {
        dstPtr[0] = 0.299f * srcPtr[0] + 0.587f * srcPtr[1] + 0.114f * srcPtr[2];
    }
}
static void RGBA32FToA32F(const byte *src, byte *dst, int numPixels, bool isGamma) {
    const float *srcPtr = (const float *)src;
    const float *srcEnd = srcPtr + numPixels * 4;
    float *dstPtr = (float *)dst;

    for (; srcPtr < srcEnd; srcPtr += 4, dstPtr += 1) {
        dstPtr[0] = srcPtr[3];
    }
}
static void RGBA32FToLA32F(const byte *src, byte *dst, int numPixels, bool isGamma) {
    const float *srcPtr = (const float *)src;
    const float *srcEnd = srcPtr + numPixels * 4;
    float *dstPtr = (float *)dst;

    for (; srcPtr < srcEnd; srcPtr += 4, dstPtr += 2) {
        dstPtr[0] = 0.299f * srcPtr[0] + 0.587f * srcPtr[1] + 0.114f * srcPtr[2];
        dstPtr[1] = srcPtr[3];
    }
}
static void RGBA32FToR32F(const byte *src, byte *dst, int numPixels, bool isGamma) {
    const float *srcPtr = (const float *)src;
    const float *srcEnd = srcPtr + numPixels * 4;
    float *dstPtr = (float *)dst;

    for (; srcPtr < srcEnd; srcPtr += 4, dstPtr += 1) {
        dstPtr[0] = srcPtr[0];
    }
}
static void RGBA32FToRG32F(const byte *src, byte *dst, int numPixels, bool isGamma) {
    const float *srcPtr = (const float *)src;
    const float *srcEnd = srcPtr + numPixels * 4;
    float *dstPtr = (float *)dst;

    for (; srcPtr < srcEnd; srcPtr += 4, dstPtr += 2) {
        dstPtr[0] = srcPtr[0];
        dstPtr[1] = srcPtr[1];
    }
}
static void RGBA32FToRGB32F(const byte *src, byte *dst, int numPixels, bool isGamma) {
    const float *srcPtr = (const float *)src;
    const float *srcEnd = srcPtr + numPixels * 4;
    float *dstPtr = (float *)dst;

    for (; srcPtr < srcEnd; srcPtr += 4, dstPtr += 3) {
        dstPtr[0] = srcPtr[0];
        dstPtr[1] = srcPtr[1];
        dstPtr[2] = srcPtr[2];
    }
}
static void RGBA32FToRGB11F11F10F(const byte *src, byte *dst, int numPixels, bool isGamma) {
    const float *srcPtr = (const float *)src;
    const float *srcEnd = srcPtr + numPixels * 4;
    uint32_t *dstPtr = (uint32_t *)dst;

    for (; srcPtr < srcEnd; srcPtr += 4, dstPtr++) {
        uint32_t r = F11Converter::FromF32(srcPtr[0]);
        uint32_t g = F11Converter::FromF32(srcPtr[1]);
        uint32_t b = F10Converter::FromF32(srcPtr[2]);
        *dstPtr = r | (g << 11) | (b << 22);
    }
}

// 컬러 채널 표기(RGBALX)는 바이트 주소체계와 선형이다 (D3D식 표기와 반대)
static const ImageFormatInfo imageFormatInfo[] = {
    // +----------------------------------------------------------------------------------- name
    // |                        +---------------------------------------------------------- size (bytes per pixel or bytes per block)
    // |                        |   +------------------------------------------------------ number of components
    // |                        |   |   +-------------------------------------------------- r bits
    // |                        |   |   |   +---------------------------------------------- g bits
    // |                        |   |   |   |   +------------------------------------------ b bits
    // |                        |   |   |   |   |   +-------------------------------------- a bits
    // |                        |   |   |   |   |   |   +---------------------------------- type
    // |                        |   |   |   |   |   |   |  +------------------------------- unpackRGBA8888
    // |                        |   |   |   |   |   |   |  |        +---------------------- packRGBA8888
    // |                        |   |   |   |   |   |   |  |        |        +------------- unpackRGBA32F
    // |                        |   |   |   |   |   |   |  |        |        |        +---- packRGBA32F
    // |                        |   |   |   |   |   |   |  |        |        |        |
    { "NULL",                   0,  0,  0,  0,  0,  0,  0, nullptr, nullptr, nullptr, nullptr },
    { "L_8",                    1,  1,  0,  0,  0,  0,  0, L8ToRGBA8888, RGBA8888ToL8, L8ToRGBA32F, RGBA32FToL8 },
    { "A_8",                    1,  1,  0,  0,  0,  8,  0, A8ToRGBA8888, RGBA8888ToA8, A8ToRGBA32F, RGBA32FToA8 },
    { "LA_8_8",                 2,  2,  0,  0,  0,  8,  0, LA88ToRGBA8888, RGBA8888ToLA88, LA88ToRGBA32F, RGBA32FToLA88 },
    { "LA_16_16",               4,  2,  0,  0,  0,  16, 0, LA1616ToRGBA8888, RGBA8888ToLA1616, LA1616ToRGBA32F, RGBA32FToLA1616 },
    { "R_8",                    1,  1,  8,  0,  0,  0,  0, R8ToRGBA8888, RGBA8888ToR8, R8ToRGBA32F, RGBA32FToR8 },
    { "R_SNORM_8",              1,  1,  8,  0,  0,  0,  0, nullptr/*R8ToRGBA8888*/, nullptr/*RGBA8888ToR8*/, nullptr, nullptr },
    { "RG_8_8",                 2,  2,  8,  8,  0,  0,  0, RG88ToRGBA8888, RGBA8888ToRG88, RG88ToRGBA32F, RGBA32FToRG88 },
    { "RG_SNORM_8_8",           2,  2,  8,  8,  0,  0,  0, nullptr/*RG88ToRGBA8888*/, nullptr/*RGBA8888ToRG88*/, nullptr, nullptr },
    { "RGB_8_8_8",              3,  3,  8,  8,  8,  0,  0, RGB888ToRGBA8888, RGBA8888ToRGB888, RGB888ToRGBA32F, RGBA32FToRGB888 },
    { "RGB_SNORM_8_8_8",        3,  3,  8,  8,  8,  0,  0, nullptr/*RGB888ToRGBA8888*/, nullptr/*RGBA8888ToRGB888*/, nullptr, nullptr },
    { "BGR_8_8_8",              3,  3,  8,  8,  8,  0,  0, BGR888ToRGBA8888, RGBA8888ToBGR888, BGR888ToRGBA32F, RGBA32FToBGR888 },
    { "RGBX_8_8_8_8",           4,  4,  8,  8,  8,  0,  0, RGBX8888ToRGBA8888, RGBA8888ToRGBX8888, RGBX8888ToRGBA32F, RGBA32FToRGBX8888 },
    { "BGRX_8_8_8_8",           4,  4,  8,  8,  8,  0,  0, BGRX8888ToRGBA8888, RGBA8888ToBGRX8888, BGRX8888ToRGBA32F, RGBA32FToBGRX8888 },
    { "RGBA_8_8_8_8",           4,  4,  8,  8,  8,  8,  0, RGBA8888ToRGBA8888, RGBA8888ToRGBA8888, RGBA8888ToRGBA32F, RGBA32FToRGBA8888 },
    { "RGBA_SNORM_8_8_8_8",     3,  4,  8,  8,  8,  0,  0, nullptr/*RGB888ToRGBA8888*/, nullptr /*RGBA8888ToRGB888*/, nullptr, nullptr },
    { "BGRA_8_8_8_8",           4,  4 , 8,  8,  8,  8,  0, BGRA8888ToRGBA8888, RGBA8888ToBGRA8888, BGRA8888ToRGBA32F, RGBA32FToBGRA8888 },
    { "ABGR_8_8_8_8",           4,  4,  8,  8,  8,  8,  0, ABGR8888ToRGBA8888, RGBA8888ToABGR8888, ABGR8888ToRGBA32F, RGBA32FToABGR8888 },
    { "ARGB_8_8_8_8",           4,  4,  8,  8,  8,  8,  0, ARGB8888ToRGBA8888, RGBA8888ToARGB8888, ARGB8888ToRGBA32F, RGBA32FToARGB8888 },
    // packed format ------------------------------------------------------------------------------
    { "RGBX_4_4_4_4",           2,  4,  4,  4,  4,  0,  Image::FormatType::Packed, RGBX4444ToRGBA8888, RGBA8888ToRGBX4444, nullptr, nullptr },
    { "BGRX_4_4_4_4",           2,  4,  4,  4,  4,  0,  Image::FormatType::Packed, BGRX4444ToRGBA8888, RGBA8888ToBGRX4444, nullptr, nullptr },
    { "RGBA_4_4_4_4",           2,  4,  4,  4,  4,  4,  Image::FormatType::Packed, RGBA4444ToRGBA8888, RGBA8888ToRGBA4444, nullptr, nullptr },
    { "BGRA_4_4_4_4",           2,  4,  4,  4,  4,  4,  Image::FormatType::Packed, BGRA4444ToRGBA8888, RGBA8888ToBGRA4444, nullptr, nullptr },
    { "ABGR_4_4_4_4",           2,  4,  4,  4,  4,  4,  Image::FormatType::Packed, ABGR4444ToRGBA8888, RGBA8888ToABGR4444, nullptr, nullptr },
    { "ARGB_4_4_4_4",           2,  4,  4,  4,  4,  4,  Image::FormatType::Packed, ARGB4444ToRGBA8888, RGBA8888ToARGB4444, nullptr, nullptr },
    { "RGBX_5_5_5_1",           2,  4,  5,  5,  5,  0,  Image::FormatType::Packed, RGBX5551ToRGBA8888, RGBA8888ToRGBX5551, nullptr, nullptr },
    { "BGRX_5_5_5_1",           2,  4,  5,  5,  5,  0,  Image::FormatType::Packed, BGRX5551ToRGBA8888, RGBA8888ToBGRX5551, nullptr, nullptr },
    { "RGBA_5_5_5_1",           2,  4,  5,  5,  5,  1,  Image::FormatType::Packed, RGBA5551ToRGBA8888, RGBA8888ToRGBA5551, nullptr, nullptr },
    { "BGRA_5_5_5_1",           2,  4,  5,  5,  5,  1,  Image::FormatType::Packed, BGRA5551ToRGBA8888, RGBA8888ToBGRA5551, nullptr, nullptr },
    { "ABGR_1_5_5_5",           2,  4,  5,  5,  5,  1,  Image::FormatType::Packed, ABGR1555ToRGBA8888, RGBA8888ToABGR1555, nullptr, nullptr },
    { "ARGB_1_5_5_5",           2,  4,  5,  5,  5,  1,  Image::FormatType::Packed, ARGB1555ToRGBA8888, RGBA8888ToARGB1555, nullptr, nullptr },
    { "RGB_5_6_5",              2,  3,  5,  6,  5,  0,  Image::FormatType::Packed, RGB565ToRGBA8888, RGBA8888ToRGB565, nullptr, nullptr },
    { "BGR_5_6_5",              2,  3,  5,  6,  5,  0,  Image::FormatType::Packed, BGR565ToRGBA8888, RGBA8888ToBGR565, nullptr, nullptr },
    // float format -------------------------------------------------------------------------------
    { "L_16F",                  2,  1,  0,  0,  0,  0,  Image::FormatType::Half, L16FToRGBA8888, RGBA8888ToL16F, L16FToRGBA32F, RGBA32FToL16F },
    { "A_16F",                  2,  1,  0,  0,  0,  16, Image::FormatType::Half, A16FToRGBA8888, RGBA8888ToA16F, A16FToRGBA32F, RGBA32FToA16F },
    { "LA_16F_16F",             4,  2,  0,  0,  0,  16, Image::FormatType::Half, LA16FToRGBA8888, RGBA8888ToLA16F, LA16FToRGBA32F, RGBA32FToLA16F },
    { "R_16F",                  2,  1,  16, 0,  0,  0,  Image::FormatType::Half, R16FToRGBA8888, RGBA8888ToR16F, R16FToRGBA32F, RGBA32FToR16F },
    { "RG_16F_16F",             4,  2,  16, 16, 0,  0,  Image::FormatType::Half, RG16FToRGBA8888, RGBA8888ToRG16F, RG16FToRGBA32F, RGBA32FToRG16F },
    { "RGB_16F_16F_16F",        6,  3,  16, 16, 16, 0,  Image::FormatType::Half, RGB16FToRGBA8888, RGBA8888ToRGB16F, RGB16FToRGBA32F, RGBA32FToRGB16F },
    { "RGBA_16F_16F_16F_16F",   8,  4,  16, 16, 16, 16, Image::FormatType::Half, RGBA16FToRGBA8888, RGBA8888ToRGBA16F, RGBA16FToRGBA32F, RGBA32FToRGBA16F },
    { "L_32F",                  4,  1,  0,  0,  0,  0,  Image::FormatType::Float, L32FToRGBA8888, RGBA8888ToL32F, L32FToRGBA32F, RGBA32FToL32F },
    { "A_32F",                  4,  1,  0,  0,  0,  32, Image::FormatType::Float, A32FToRGBA8888, RGBA8888ToA32F, A32FToRGBA32F, RGBA32FToA32F },
    { "LA_32F_32F",             8,  2,  0,  0,  0,  32, Image::FormatType::Float, LA32FToRGBA8888, RGBA8888ToLA32F, LA32FToRGBA32F, RGBA32FToLA32F },
    { "R_32F",                  4,  1,  32, 0,  0,  0,  Image::FormatType::Float, R32FToRGBA8888, RGBA8888ToR32F, R32FToRGBA32F, RGBA32FToR32F },
    { "RG_32F_32F",             8,  2,  32, 32, 0,  0,  Image::FormatType::Float, RG32FToRGBA8888, RGBA8888ToRG32F, RG32FToRGBA32F, RGBA32FToRG32F },
    { "RGB_32F_32F_32F",        12, 3,  32, 32, 32, 0,  Image::FormatType::Float, RGB32FToRGBA8888, RGBA8888ToRGB32F, RGB32FToRGBA32F, RGBA32FToRGB32F },
    { "RGBA_32F_32F_32F_32F",   16, 4,  32, 32, 32, 32, Image::FormatType::Float, RGBA32FToRGBA8888, RGBA8888ToRGBA32F, RGBA32FToRGBA32F, RGBA32FToRGBA32F },
    { "RGB_11F_11F_10F",        4,  3,  11, 11, 10, 0,  Image::FormatType::Float | Image::FormatType::Packed, RGB11F11F10FToRGBA8888, RGBA8888ToRGB11F11F10F, RGB11F11F10FToRGBA32F, RGBA32FToRGB11F11F10F },
    { "RGBE_9_9_9_5",           4,  3,  9,  9,  9,  0,  Image::FormatType::Float | Image::FormatType::Packed, RGBE9995ToRGBA8888, RGBA8888ToRGBE9995, RGBE9995ToRGBA32F, RGBA32FToRGBE9995 },
    // DXT (BTC) ----------------------------------------------------------------------------------
    { "DXT1",                   8,  4,  0,  0,  0,  0,  Image::FormatType::Compressed, nullptr, nullptr, nullptr, nullptr },
    { "DXT3",                   16, 4,  0,  0,  0,  0,  Image::FormatType::Compressed, nullptr, nullptr, nullptr, nullptr },
    { "DXT5",                   16, 4,  0,  0,  0,  0,  Image::FormatType::Compressed, nullptr, nullptr, nullptr, nullptr },
    { "DXT5_RXGB",              16, 4,  0,  0,  0,  0,  Image::FormatType::Compressed, nullptr, nullptr, nullptr, nullptr },
    { "DXN1",                   8,  2,  0,  0,  0,  0,  Image::FormatType::Compressed, nullptr, nullptr, nullptr, nullptr },
    { "DXN2",                   16, 2,  0,  0,  0,  0,  Image::FormatType::Compressed, nullptr, nullptr, nullptr, nullptr },
    // PVRTC --------------------------------------------------------------------------------------
    { "RGB_PVRTC_2BPPV1",       8,  3,  0,  0,  0,  0,  Image::FormatType::Compressed, nullptr, nullptr, nullptr, nullptr },
    { "RGB_PVRTC_4BPPV1",       8,  3,  0,  0,  0,  0,  Image::FormatType::Compressed, nullptr, nullptr, nullptr, nullptr },
    { "RGBA_PVRTC_2BPPV1",      8,  4,  0,  0,  0,  0,  Image::FormatType::Compressed, nullptr, nullptr, nullptr, nullptr },
    { "RGBA_PVRTC_4BPPV1",      8,  4,  0,  0,  0,  0,  Image::FormatType::Compressed, nullptr, nullptr, nullptr, nullptr },
    { "RGBA_PVRTC_2BPPV2",      8,  4,  0,  0,  0,  0,  Image::FormatType::Compressed, nullptr, nullptr, nullptr, nullptr },
    { "RGBA_PVRTC_4BPPV2",      8,  4,  0,  0,  0,  0,  Image::FormatType::Compressed, nullptr, nullptr, nullptr, nullptr },
    // ETC ----------------------------------------------------------------------------------------
    { "RGB_8_ETC1",             8,  3,  0,  0,  0,  0,  Image::FormatType::Compressed, nullptr, nullptr, nullptr, nullptr },
    { "RGB_8_ETC2",             8,  3,  0,  0,  0,  0,  Image::FormatType::Compressed, nullptr, nullptr, nullptr, nullptr },
    { "RGBA_8_1_ETC2",          8,  4,  0,  0,  0,  0,  Image::FormatType::Compressed, nullptr, nullptr, nullptr, nullptr },
    { "RGBA_8_8_ETC2",          16, 4,  0,  0,  0,  0,  Image::FormatType::Compressed, nullptr, nullptr, nullptr, nullptr },
    { "R_11_EAC",               8,  1,  0,  0,  0,  0,  Image::FormatType::Compressed, nullptr, nullptr, nullptr, nullptr },
    { "RG_11_11_EAC",           16, 2,  0,  0,  0,  0,  Image::FormatType::Compressed, nullptr, nullptr, nullptr, nullptr },
    { "SignedR_11_EAC",         8,  1,  0,  0,  0,  0,  Image::FormatType::Compressed, nullptr, nullptr, nullptr, nullptr },
    { "SignedRG_11_11_EAC",     16, 2,  0,  0,  0,  0,  Image::FormatType::Compressed, nullptr, nullptr, nullptr, nullptr },
    // ATC ----------------------------------------------------------------------------------------
    { "RGB_ATC",                8,  3,  0,  0,  0,  0,  Image::FormatType::Compressed, nullptr, nullptr, nullptr, nullptr },
    { "RGBA_EA_ATC",            16, 4,  0,  0,  0,  0,  Image::FormatType::Compressed, nullptr, nullptr, nullptr, nullptr },
    { "RGBA_IA_ATC",            16, 4,  0,  0,  0,  0,  Image::FormatType::Compressed, nullptr, nullptr, nullptr, nullptr },
    // depth --------------------------------------------------------------------------------------
    { "Depth_16",               2,  1,  0,  0,  0,  0,  Image::FormatType::Depth, nullptr, nullptr, nullptr, nullptr },
    { "Depth_24",               3,  1,  0,  0,  0,  0,  Image::FormatType::Depth, nullptr, nullptr, nullptr, nullptr },
    { "Depth_32F",              4,  1,  0,  0,  0,  0,  Image::FormatType::Depth | Image::FormatType::Float, nullptr, nullptr, nullptr, nullptr },
    { "DepthStencil_24_8",      4,  2,  0,  0,  0,  0,  Image::FormatType::DepthStencil, nullptr, nullptr, nullptr, nullptr },
    { "DepthStencil_32F_8",     5,  2,  0,  0,  0,  0,  Image::FormatType::DepthStencil, nullptr, nullptr, nullptr, nullptr },
};

const ImageFormatInfo *GetImageFormatInfo(Image::Format::Enum imageFormat) {
    assert(imageFormat < Image::Format::Count);
    return &imageFormatInfo[imageFormat];
}

bool CompressedFormatMinDimensions(Image::Format::Enum imageFormat, int &minWidth, int &minHeight) {
    switch (imageFormat) {
    case Image::Format::RGBA_DXT1:
    case Image::Format::RGBA_DXT3:
    case Image::Format::RGBA_DXT5:
    case Image::Format::XGBR_DXT5:
    case Image::Format::DXN1:
    case Image::Format::DXN2:
        minWidth = 4;
        minHeight = 4;
        return true;
    case Image::Format::RGB_PVRTC_2BPPV1:
    case Image::Format::RGBA_PVRTC_2BPPV1:
        minWidth = 16;
        minHeight = 8;
        return true;
    case Image::Format::RGB_PVRTC_4BPPV1:
    case Image::Format::RGBA_PVRTC_4BPPV1:
        minWidth = 8;
        minHeight = 8;
        return true;
    case Image::Format::RGBA_PVRTC_2BPPV2:
        minWidth = 8;
        minHeight = 4;
        return true;
    case Image::Format::RGBA_PVRTC_4BPPV2:
        minWidth = 4;
        minHeight = 4;
        return true;
    case Image::Format::RGB_8_ETC1:
    case Image::Format::RGB_8_ETC2:
    case Image::Format::RGBA_8_1_ETC2:
    case Image::Format::RGBA_8_8_ETC2:
    case Image::Format::R_11_EAC:
    case Image::Format::SignedR_11_EAC:
    case Image::Format::RG_11_11_EAC:
    case Image::Format::SignedRG_11_11_EAC:
        minWidth = 4;
        minHeight = 4;
        return true;
    case Image::Format::RGB_ATC:
    case Image::Format::RGBA_EA_ATC: // Explicit alpha
    case Image::Format::RGBA_IA_ATC: // Interpolated alpha
        minWidth = 4;
        minHeight = 4;
        return false;
    default:
        return false;
    }
    
    return false;
}

bool CompressedFormatBlockDimensions(Image::Format::Enum imageFormat, int &blockWidth, int &blockHeight) {
    switch (imageFormat) {
    case Image::Format::RGBA_DXT1:
    case Image::Format::RGBA_DXT3:
    case Image::Format::RGBA_DXT5:
    case Image::Format::XGBR_DXT5:
    case Image::Format::DXN1:
    case Image::Format::DXN2:
        blockWidth = 4;
        blockHeight = 4;
        return true;
    case Image::Format::RGB_PVRTC_2BPPV1:
    case Image::Format::RGBA_PVRTC_2BPPV1:
        blockWidth = 8;
        blockHeight = 4;
        return true;
    case Image::Format::RGB_PVRTC_4BPPV1:
    case Image::Format::RGBA_PVRTC_4BPPV1:
        blockWidth = 4;
        blockHeight = 4;
        return true;
    case Image::Format::RGBA_PVRTC_2BPPV2:
        blockWidth = 4;
        blockHeight = 4;
        return true;
    case Image::Format::RGBA_PVRTC_4BPPV2:
        blockWidth = 4;
        blockHeight = 4;
        return true;
    case Image::Format::RGB_8_ETC1:
    case Image::Format::RGB_8_ETC2:
    case Image::Format::RGBA_8_1_ETC2:
    case Image::Format::RGBA_8_8_ETC2:
    case Image::Format::R_11_EAC:
    case Image::Format::SignedR_11_EAC:
    case Image::Format::RG_11_11_EAC:
    case Image::Format::SignedRG_11_11_EAC:
        blockWidth = 4;
        blockHeight = 4;
        return true;
    case Image::Format::RGB_ATC:
    case Image::Format::RGBA_EA_ATC: // Explicit alpha
    case Image::Format::RGBA_IA_ATC: // Interpolated alpha
        blockWidth = 4;
        blockHeight = 4;
        return false;
    default:
        return false;
    }
    
    return false;
}

BE_NAMESPACE_END
