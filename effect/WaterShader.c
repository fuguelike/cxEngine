//
//  WaterShader.c
//  cxEngine
//
//  Created by xuhua on 3/2/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#include <engine/cxView.h>
#include <engine/cxEngine.h>
#include "WaterShader.h"
cxStr getvs()
{
    return cxAssetsData("aoe.vsh");
}
cxStr getfs()
{
    return cxAssetsData("aoe.fsh");;
}
//cxConstChars getfs()
//{
//    static cxConstChars fs =
//    GLSL(
//        precision highp float;
//        varying mediump vec4 vFragmentColor;
//        varying highp vec2 vTexCoord;
//        uniform sampler2D uTexture0;
//        uniform vec2 center;
//        uniform vec2 resolution;
//        uniform float time;
//        const float TAU = 6.28318530718;
//        const float MAX_ITER = 3.;
//        void main(){
//            float ctime = time * .5+23.0;
//            vec2 uv = gl_FragCoord.xy / resolution.xy;
//            vec2 p = mod(uv*TAU, TAU)-250.0;
//            vec2 i = vec2(p);
//            float c = 1.0;
//            float inten = .005;
//            for (int n = 0; n < int(MAX_ITER); n++){
//                float t = ctime * (1.0 - (3.5 / float(n+1)));
//                i = p + vec2(cos(t - i.x) + sin(t + i.y), sin(t - i.y) + cos(t + i.x));
//                c += 1.0/length(vec2(p.x / (sin(i.x+t)/inten),p.y / (cos(i.y+t)/inten)));
//            }
//            c /= float(MAX_ITER);
//            c = 1.17-pow(c, 1.4);
//            vec3 colour = vec3(pow(abs(c), 8.0));
//            colour = clamp(colour + vec3(0.0, 0.35, 0.5), 0.0, 1.0);
//            
//            vec2 pixel = 4.0 / resolution.xy;
//            uv *= 4.0;
//            vec2 first = step(pixel, uv);		   	// Rule out first screen pixels and flash.
//            uv  = step(fract(uv), pixel);
//            colour = mix(colour, vec3(1.0, 1.0, 0.0), (uv.x + uv.y) * first.x * first.y); // Yellow line
//            
//            gl_FragColor = vec4(colour, 1.0);
//        });
//    return fs;
//}

//线条动画
//CX_METHOD_DEF(WaterShader, Fragment,cxStr)
//{
//    static cxConstChars fragment =
//    GLSL(
//         precision highp float;
//         varying mediump vec4 vFragmentColor;
//         varying highp vec2 vTexCoord;
//         uniform sampler2D uTexture0;
//         uniform vec2 center;
//         uniform vec2 resolution;
//         uniform float time;
//         void main(){
//            vec2 uv = vTexCoord;
//            vec2 p = uv*3.0 - 1.0;
//            p *= 8.0;
//            float sfunc = p.y + 5.0*sin(uv.x*10.0-time*2.0 + cos(time*2.0) )+2.0*cos(uv.x*25.0+time*10.0);
//            sfunc *= uv.x*2.0+0.3;
//            sfunc *= 2.0 - uv.x*2.0+0.3;
//            sfunc /= 1.0; // Thickness fix
//
//            vec3 c = vec3(abs(sfunc));
//            c = pow(c, vec3(-1.0));
//            c *= vec3(0.8,0.2,0.0);
//            gl_FragColor = vec4(c,1.0);
//    });
//    return cxStrConstChars(fragment);
//}
CX_METHOD_DEF(WaterShader, Vertex,cxStr)
{
    static cxConstChars vertex =
    GLSL(
         attribute highp vec3 aPosition;
         attribute mediump vec2 aTexcoord;
         attribute mediump vec4 aColor;
         varying mediump vec4 vFragmentColor;
         varying mediump vec2 vTexCoord;
         void main() {
             gl_Position = cxMatrixModelViewProject * vec4(aPosition,1.0);
             vFragmentColor = aColor;
             vTexCoord = aTexcoord;
         });
    return cxStrConstChars(vertex);
}
CX_METHOD_DEF(WaterShader, Fragment,cxStr)
{
    static cxConstChars fragment =
    GLSL(
         varying mediump vec4 vFragmentColor;
         varying highp vec2 vTexCoord;
         uniform sampler2D uTexture0;
         uniform vec2 center;
         uniform vec2 resolution;
         uniform float time;
         void main(){
             vec4 color = texture2D(uTexture0, vTexCoord);
             color.xyz = (dot(color.xyz, vec3(0.3, 0.59, 0.11)) * vec3(0.8, 0.75, 0.55)) + vec3(0.3, 0.2, 0.18);
             gl_FragColor = color;
         }
    );
    return cxStrConstChars(fragment);
}
// fire sea shader
//CX_METHOD_DEF(WaterShader, Fragment,cxStr)
//{
//    static cxConstChars fragment =
//    GLSL(
//         precision highp float;
//         varying mediump vec4 vFragmentColor;
//         varying highp vec2 vTexCoord;
//         uniform sampler2D uTexture0;
//         uniform vec2 center;
//         uniform vec2 resolution;
//         uniform float time;
//         // much simplified version of the fire shader by @301z
//         
//         const float SCALE = 16.0;
//         // speed of perturbations
//         const float PSPEED1 = 1.8;
//         const float PSPEED2 = 1.0;
//         // speed of flame cores
//         const vec2 CORESPEED = vec2(0.2,0.4);
//         const vec3 color1 = vec3(0.5, 0.1, 0.0);
//         const vec3 color2 = vec3(1.0, 0.5, 0.0);
//         float rand(vec2 n) {
//             return fract(sin(dot(n, vec2(12.9898, 4.1414))) * 43758.5453);
//         }
//         float noise(vec2 n) {
//             const vec2 d = vec2(0.0, 1.0);
//             vec2 b = floor(n);
//             vec2 f = smoothstep(vec2(0.0), vec2(1.0), fract(n));
//             return mix(mix(rand(b), rand(b + d.yx), f.x), mix(rand(b + d.xy), rand(b + d.yy), f.x), f.y);
//         }
//         void main() {
//             vec2 p = gl_FragCoord.xy * SCALE / resolution.xx;
//             vec2 r = vec2(noise(p + time*PSPEED1 - p.x - p.y), noise(p - time*PSPEED2));
//             vec3 c = mix(color1, color2, noise(p + r - CORESPEED*time));
//             gl_FragColor = vec4(c, 1.0);
//         });
//    return cxStrConstChars(fragment);
//}
//CX_METHOD_DEF(WaterShader, Fragment,cxStr)
//{
//    static cxConstChars fragment =
//    GLSL(
//         precision highp float;
//         varying mediump vec4 vFragmentColor;
//         varying highp vec2 vTexCoord;
//         uniform sampler2D uTexture0;
//         uniform vec2 center;
//         uniform vec2 resolution;
//         uniform float time;
//         const float PI = 3.14159265359;
//         float noise (vec2 p) {
//             return texture2D (uTexture0, p).r;
//         }
//         float fbm (vec2 p) {
//             float f = 0.0;
//             float q = 0.0;
//             float a = 0.0;
//             for (float i = 1.0; i <= 5.0; ++i) {
//                 q = pow (2.0, i);
//                 a = a + 1.0/q;
//                 f = noise (p * (q + i/10.0))/q + f;
//             }
//             return f/a;
//         }
//         vec3 noise3v (vec2 p) {
//             return texture2D (uTexture0, p).rgb;
//         }
//         vec3 fbm3v (vec2 p) {
//             vec3 f = vec3 (0.0);
//             float q = 0.0;
//             float a = 0.0;
//             for (float i = 1.0; i <= 5.0; ++i) {
//                 q = pow (2.0, i);
//                 a = a + 1.0/q;
//                 f = noise (p * (q + i/10.0))/q + f;
//             }
//             return f/a;
//         }
//         void main() {
//             vec2 uv = (2.0 * gl_FragCoord.xy - resolution.xy)/min (resolution.x, resolution.y);
//             vec2 uv0 = uv;
//             vec3 nd = fbm3v (uv/256.0 + vec2 (1.0, 0.0) * time/1024.0)-0.5;
//             uv = uv+0.5*nd.xy;
//             float nv = fbm (uv/256.0 + vec2 (0.0, -1.0) * time/1024.0);
//             float imp = abs (sin ((nv + time/10.0)*PI*5.0));
//             vec3 uv1 = normalize (vec3 (uv0, 1.0) + 0.1*nd);
//             gl_FragColor = vec4 (pow (imp, 1.4), pow (imp, 5.2), pow (imp, 9.3), 1.0);
//         });
//    return cxStrConstChars(fragment);
//}
CX_METHOD_DEF(WaterShader,Update,void,cxAny pview)
{
    cxSize2f size = cxViewGetSize(pview);
    glUniform1f(this->time, cxEngineTime());
    glUniform2f(this->center, 0.5f, 0.5f);
    glUniform2f(this->resolution, size.w/2.0f, size.h/2.0f);
}
CX_METHOD_DEF(WaterShader,Uniform,void)
{
    this->center = glGetUniformLocation(this->cxShader.program, "center");
    this->resolution = glGetUniformLocation(this->cxShader.program, "resolution");
    this->time = glGetUniformLocation(this->cxShader.program, "time");
}
CX_TYPE(WaterShader, cxShader)
{
    CX_METHOD(WaterShader, Update);
    CX_METHOD(WaterShader, Uniform);
    CX_METHOD(WaterShader, Vertex);
    CX_METHOD(WaterShader, Fragment);
}
CX_INIT(WaterShader, cxShader)
{
    
}
CX_FREE(WaterShader, cxShader)
{
    
}
CX_TERM(WaterShader, cxShader)