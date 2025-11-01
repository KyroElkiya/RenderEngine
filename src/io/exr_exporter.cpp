#pragma once

#include "../core/color.h"
#include <vector>
#include <string>
#include "pixel.h"

#include <Imath/ImathBox.h>
#include <OpenEXR/ImfRgbaFile.h>
#include <OpenEXR/ImfArray.h>
#include <OpenEXR/ImfChannelList.h>
#include <OpenEXR/ImfFrameBuffer.h>
#include <OpenEXR/ImfOutputFile.h>


inline void write_exr16(const std::string& filename, int width, int height, const std::vector<std::vector<pixel>>& framebuffer) {
    
    Imf::Header header(width, height);
    
    header.channels().insert("R", Imf::Channel(Imf::HALF));
    header.channels().insert("G", Imf::Channel(Imf::HALF));
    header.channels().insert("B", Imf::Channel(Imf::HALF));
    header.channels().insert("A", Imf::Channel(Imf::HALF));

    header.channels().insert("N.red", Imf::Channel(Imf::HALF));
    header.channels().insert("N.green", Imf::Channel(Imf::HALF));
    header.channels().insert("N.blue", Imf::Channel(Imf::HALF));
    header.channels().insert("depth.Z", Imf::Channel(Imf::HALF));

    header.channels().insert("P.x", Imf::Channel(Imf::HALF));
    header.channels().insert("P.y", Imf::Channel(Imf::HALF));
    header.channels().insert("P.z", Imf::Channel(Imf::HALF));

    Imf::FrameBuffer buffer;
    
    std::vector<Imf::Rgba> rgbaPixels(height * width);
    
    std::vector<Imath::half> nx(height * width);
    std::vector<Imath::half> ny(height * width);
    std::vector<Imath::half> nz(height * width);
    std::vector<Imath::half> depth(height * width);

    std::vector<Imath::half> px(height * width);
    std::vector<Imath::half> py(height * width);
    std::vector<Imath::half> pz(height * width);

    for (int j = 0; j < height; j++)
        for (int i = 0; i < width; i++) {
            int idx = j * width + i;
            const pixel &curr_pixel = framebuffer[j][i];

            vec3 col = return_srgb(curr_pixel.rgb);
            vec3 norm = curr_pixel.N;

            rgbaPixels[idx].r = col.x;
            rgbaPixels[idx].g = col.y;
            rgbaPixels[idx].b = col.z;
            rgbaPixels[idx].a = curr_pixel.hit;
            
            nx[idx] = Imath::half(curr_pixel.N.x);
            ny[idx] = Imath::half(curr_pixel.N.y);
            nz[idx] = Imath::half(curr_pixel.N.z);
            depth[idx] = Imath::half(curr_pixel.depth);
            
            px[idx] = Imath::half(curr_pixel.P.x);
            py[idx] = Imath::half(curr_pixel.P.y);
            pz[idx] = Imath::half(curr_pixel.P.z);
            }

    buffer.insert("R", Imf::Slice(Imf::HALF, (char*)&rgbaPixels[0].r, sizeof(Imf::Rgba), sizeof(Imf::Rgba)*width));
    buffer.insert("G", Imf::Slice(Imf::HALF, (char*)&rgbaPixels[0].g, sizeof(Imf::Rgba), sizeof(Imf::Rgba)*width));
    buffer.insert("B", Imf::Slice(Imf::HALF, (char*)&rgbaPixels[0].b, sizeof(Imf::Rgba), sizeof(Imf::Rgba)*width));
    buffer.insert("A", Imf::Slice(Imf::HALF, (char*)&rgbaPixels[0].a, sizeof(Imf::Rgba), sizeof(Imf::Rgba)*width));

    buffer.insert("N.red", Imf::Slice(Imf::HALF, (char*)nx.data(), sizeof(Imath::half), sizeof(Imath::half)*width));
    buffer.insert("N.green", Imf::Slice(Imf::HALF, (char*)ny.data(), sizeof(Imath::half), sizeof(Imath::half)*width));
    buffer.insert("N.blue", Imf::Slice(Imf::HALF, (char*)nz.data(), sizeof(Imath::half), sizeof(Imath::half)*width));
    buffer.insert("depth.Z", Imf::Slice(Imf::HALF, (char*)depth.data(), sizeof(Imath::half), sizeof(Imath::half)*width));

    buffer.insert("P.x", Imf::Slice(Imf::HALF, (char*)px.data(), sizeof(Imath::half), sizeof(Imath::half)*width));
    buffer.insert("P.y", Imf::Slice(Imf::HALF, (char*)py.data(), sizeof(Imath::half), sizeof(Imath::half)*width));
    buffer.insert("P.z", Imf::Slice(Imf::HALF, (char*)pz.data(), sizeof(Imath::half), sizeof(Imath::half)*width));

    Imf::OutputFile file(filename.c_str(), header);           
    file.setFrameBuffer(buffer);
    file.writePixels(height);
}
