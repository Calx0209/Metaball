#pragma once

#include "mesh.hpp"
#include "metaball.hpp"

struct MarchingCube
{
  Mesh mesh;

  MarchingCube(Metaball &metaball) noexcept
  {
    for (std::size_t iz = 0; iz < metaball.grid.z.size - 1; ++iz)
    {
      auto const z1 = metaball.grid.z[iz];
      auto const z2 = metaball.grid.z[iz + 1];

      for (std::size_t iy = 0; iy < metaball.grid.y.size - 1; ++iy)
      {
        auto const y1 = metaball.grid.y[iy];
        auto const y2 = metaball.grid.y[iy + 1];

        for (std::size_t ix = 0; ix < metaball.grid.x.size - 1; ++ix)
        {
          auto const x1 = metaball.grid.x[ix];
          auto const x2 = metaball.grid.x[ix + 1];
          
          auto const p1v = metaball[{ix, iy, iz + 1}];
          auto const p2v = metaball[{ix + 1, iy, iz + 1}];
          auto const p3v = metaball[{ix + 1, iy, iz}];
          auto const p4v = metaball[{ix, iy, iz}];
          auto const p5v = metaball[{ix, iy + 1, iz + 1}];
          auto const p6v = metaball[{ix + 1, iy + 1, iz + 1}];
          auto const p7v = metaball[{ix + 1, iy + 1, iz}];
          auto const p8v = metaball[{ix, iy + 1, iz}];

          auto const t12 = calc_t(p1v, p2v);
          auto const t23 = calc_t(p2v, p3v);
          auto const t34 = calc_t(p3v, p4v);
          auto const t41 = calc_t(p4v, p1v);
          auto const t56 = calc_t(p5v, p6v);
          auto const t67 = calc_t(p6v, p7v);
          auto const t78 = calc_t(p7v, p8v);
          auto const t81 = calc_t(p8v, p1v);
          auto const t15 = calc_t(p1v, p5v);
          auto const t26 = calc_t(p2v, p6v);
          auto const t37 = calc_t(p3v, p7v);
          auto const t48 = calc_t(p4v, p8v);

          auto const l1 = lerp(x1, x2, t12);
          auto const l2 = lerp(z2, z1, t23);
          auto const l3 = lerp(x2, x1, t34);
          auto const l4 = lerp(z1, z2, t41);
          auto const l5 = lerp(x1, x2, t56);
          auto const l6 = lerp(z2, z1, t67);
          auto const l7 = lerp(x2, x1, t78);
          auto const l8 = lerp(z1, z2, t81);
          auto const l9 = lerp(y1, y2, t15);
          auto const l10 = lerp(y1, y2, t26);
          auto const l11 = lerp(y1, y2, t37);
          auto const l12 = lerp(y1, y2, t48);

          Double3 const v1 = {l1, y1, z2};
          Double3 const v2 = {x2, y1, l2};
          Double3 const v3 = {l3, y1, z1};
          Double3 const v4 = {x1, y1, l4};
          Double3 const v5 = {l5, y2, z2};
          Double3 const v6 = {x2, y2, l6};
          Double3 const v7 = {l7, y2, z1};
          Double3 const v8 = {x1, y2, l8};
          Double3 const v9 = {x1, l9, z2};
          Double3 const v10 = {x2, l10, z2};
          Double3 const v11 = {x2, l11, z1};
          Double3 const v12 = {x1, l12, z1};

          int type = 0;

          if (p1v > 1)
            type += 0b1;

          if (p2v > 1)
            type += 0b10;

          if (p3v > 1)
            type += 0b100;

          if (p4v > 1)
            type += 0b1000;

          if (p5v > 1)
            type += 0b10000;

          if (p6v > 1)
            type += 0b100000;

          if (p7v > 1)
            type += 0b1000000;

          if (p8v > 1)
            type += 0b10000000;

          /*
          //         p5 -------v5--------- p6
          //        /|                    /|
          //       v8|                  v6 |
          //      /  |                  /  |
          //     /   v9                /   v10
          //    p8 --------v7-------- p7   |
          //    |    |                |    |
          //    |    |                |    |
          //    |    p1 -------v1-----|--- p2
          //    v12 /                v11  /
          //    |  v4                 |  v2
          //    | /                   | /
          //    |/                    |/
          //    p4 --------v3-------- p3
          */

          switch (type)
          {
          //------
          // CASE0:0
          case 0:
          case 255:
            break;
          //------
          
          
          //------
          // CASE1:三角形
          case 1: // 下面左上
            mesh.add_tri(v1, v4, v9);
            break;

          case 2: // 下面右上
            mesh.add_tri(v2, v1, v10);
            break;

          case 4: // 下面右下
            mesh.add_tri(v3, v2, v11);
            break;

          case 8: // 下面左下
            mesh.add_tri(v4, v3, v12);
            break;

          case 16: // 上面左上
            mesh.add_tri(v8, v5, v9);
            break;

          case 32: // 上面右上
            mesh.add_tri(v5, v6, v10);
            break;

          case 64: // 上面右下
            mesh.add_tri(v6, v7, v11);
            break;

          case 128: // 上面左下
            mesh.add_tri(v7, v8, v12);
            break;
          //------
          
          //------
          // CASE2:長方形1個
          case 12: // 下面下辺
            mesh.add_rect(v4, v12, v11, v2);
            break;
          
          case 9: // 下面左辺
            mesh.add_rect(v3, v12, v9, v1);
            break;
          
          case 3: // 下面上辺
            mesh.add_rect(v4, v9, v10, v2);
            break;

          case 6: // 下面右辺
            mesh.add_rect(v1, v10, v11, v3);
            break;

          case 48: //上面上辺
            mesh.add_rect(v6, v10, v9, v8);
            break;

          case 96: // 上面右辺
            mesh.add_rect(v7, v11, v10, v5);
            break;

          case 192: //上面下辺
            mesh.add_rect(v6, v11, v12, v8);
            break;

          case 144: // 上面左辺
            mesh.add_rect(v12, v7, v5, v9);
            break;

          case 17: // 垂線左上
            mesh.add_rect(v8, v5, v1, v4);
            break;

          case 34: // 垂線右上
            mesh.add_rect(v5, v6, v2, v1);
            break;

          case 68: // 垂線右下
            mesh.add_rect(v3, v2, v6, v7);
            break;

          case 136: // 垂線左下
            mesh.add_rect(v7, v8, v4, v3);
            break;
          //------
          

          //------
          // CASE3:対角点
          case 72:
            mesh.add_tri(v6, v7, v11);
            mesh.add_tri(v3, v4, v12);
            break;

          case 132:
            mesh.add_tri(v7, v8, v12);
            mesh.add_tri(v2, v3, v11);
            break;

          case 33:
            mesh.add_tri(v5, v6, v10);
            mesh.add_tri(v1, v4, v9);
            break;

          case 18:
            mesh.add_tri(v5, v8, v9);
            mesh.add_tri(v1, v2, v10);
            break;

          case 36:
            mesh.add_tri(v5, v6, v10);
            mesh.add_tri(v2, v3, v11);
            break;

          case 66:
            mesh.add_tri(v6, v7, v11);
            mesh.add_tri(v1, v2, v10);
            break;

          case 129:
            mesh.add_tri(v7, v8, v12);
            mesh.add_tri(v1, v4, v9);
            break;

          case 24:
            mesh.add_tri(v5, v8, v9);
            mesh.add_tri(v3, v4, v12);
            break;

          case 160:
            mesh.add_tri(v5, v6, v10);
            mesh.add_tri(v7, v8, v12);
            break;

          case 80:
            mesh.add_tri(v5, v8, v9);
            mesh.add_tri(v6, v7, v11);
            break;

          case 10:
            mesh.add_tri(v1, v2, v10);
            mesh.add_tri(v3, v4, v12);
            break;

          case 5:
            mesh.add_tri(v1, v4, v9);
            mesh.add_tri(v2, v3, v11);
            break;
          //------
          
          
          //------
          // CASE4:
          case 7:
            mesh.add_rect(v4, v9, v11, v3);
            mesh.add_tri(v9, v10, v11);
            break;

          case 14:
            mesh.add_rect(v1, v10, v12, v4);
            mesh.add_tri(v10, v11, v12);
            break;

          case 13:
            mesh.add_rect(v2, v11, v9, v1);
            mesh.add_tri(v9, v11, v12);
            break;

          case 11:
            mesh.add_rect(v3, v12, v10, v2);
            mesh.add_tri(v9, v10, v12);
            break;

          case 112:
            mesh.add_rect(v7, v11, v9, v8);
            mesh.add_tri(v9, v10, v11);
            break;

          case 224:
            mesh.add_rect(v5, v10, v12, v8);
            mesh.add_tri(v10, v11, v12);
            break;

          case 208:
            mesh.add_rect(v5, v9, v11, v6);
            mesh.add_tri(v9, v11, v12);
            break;

          case 176:
            mesh.add_rect(v6, v10, v12, v7);
            mesh.add_tri(v9, v10, v12);
            break;

          case 100:
            mesh.add_rect(v10, v5, v3, v2);
            mesh.add_tri(v5, v3, v7);
            break;

          case 70:
            mesh.add_rect(v6, v7, v1, v10);
            mesh.add_tri(v1, v3, v7);
            break;
          
          case 38:
            mesh.add_rect(v11, v3, v5, v6);
            mesh.add_tri(v1, v3, v5);
            break;
          
          case 98:
            mesh.add_rect(v2, v1, v7, v11);
            mesh.add_tri(v1, v5, v7);
            break;

          case 152:
            mesh.add_rect(v4, v3, v5, v9);
            mesh.add_tri(v3, v5, v7);
            break;

          case 137:
            mesh.add_rect(v9, v1, v7, v8);
            mesh.add_tri(v1, v3, v7);
            break;

          case 25:
            mesh.add_rect(v8, v5, v3, v12);
            mesh.add_tri(v1, v3, v5);
            break;

          case 145:
            mesh.add_rect(v12, v7, v1, v4);
            mesh.add_tri(v1, v5, v7);
            break;

          case 196:
            mesh.add_rect(v12, v8, v2, v3);
            mesh.add_tri(v2, v6, v8);
            break;

          case 76:
            mesh.add_rect(v4, v7, v6, v3);
            mesh.add_tri(v4, v7, v12);
            break;

          case 140:
            mesh.add_rect(v7, v8, v2, v11);
            mesh.add_tri(v2, v4, v8);
            break;

          case 200:
            mesh.add_rect(v11, v6, v4, v3);
            mesh.add_tri(v4, v6, v8);
            break;

          case 50:
            mesh.add_rect(v9, v8, v1, v2);
            mesh.add_tri(v2, v6, v8);
            break;

          case 35:
            mesh.add_rect(v5, v6, v4, v9);
            mesh.add_tri(v2, v4, v6);
            break;

          case 19:
            mesh.add_rect(v10, v2, v8, v5);
            mesh.add_tri(v2, v4, v8);
            break;

          case 49:
            mesh.add_rect(v1, v4, v6, v10);
            mesh.add_tri(v4, v6, v8);
            break;
          //------

          //------
          // CASE5:正方形
          case 15:
          case 240:
            mesh.add_rect(v12 ,v9, v10, v11);
            break;

          case 102:
          case 153:
            mesh.add_rect(v3, v7, v5, v1);
            break;

          case 51:
          case 204:
            mesh.add_rect(v4, v8, v6, v2);
            break;
          //------


          //------
          // CASE6:
          case 135:
            mesh.add_tri(v7, v8, v12);
            mesh.add_rect(v4, v9, v11, v3);
            mesh.add_tri(v9, v10, v11);
            break;

          case 30:
            mesh.add_tri(v5, v8, v9);
            mesh.add_rect(v1, v10, v12, v4);
            mesh.add_tri(v10, v11, v12);
            break;

          case 45:
            mesh.add_tri(v5, v6, v10);
            mesh.add_rect(v2, v11, v9, v1);
            mesh.add_tri(v9, v11, v12);
            break;

          case 75:
            mesh.add_tri(v6, v7, v11);
            mesh.add_rect(v3, v12, v10, v2);
            mesh.add_tri(v9, v10, v12);
            break;

          case 120:
            mesh.add_tri(v3, v4, v12);
            mesh.add_rect(v7, v11, v9, v8);
            mesh.add_tri(v9, v10, v11);
            break;

          case 225:
            mesh.add_tri(v1, v4, v9);
            mesh.add_rect(v5, v10, v12, v8);
            mesh.add_tri(v10, v11, v12);
            break;

          case 210:
            mesh.add_tri(v1, v2, v10);
            mesh.add_rect(v5, v9, v11, v6);
            mesh.add_tri(v9, v11, v12);
            break;

          case 180:
            mesh.add_tri(v2, v3, v11);
            mesh.add_rect(v12, v7, v6, v10);
            mesh.add_tri(v9, v10, v12);
            break;
          
          case 101:
            mesh.add_tri(v1, v4, v9);
            mesh.add_rect(v10, v5, v3, v2);
            mesh.add_tri(v3, v5, v7);
            break;

          case 86:
            mesh.add_tri(v5, v8, v9);
            mesh.add_rect(v6, v7, v1, v10);
            mesh.add_tri(v1, v3, v7);
            break;
          
          case 166:
            mesh.add_tri(v7, v8, v12);
            mesh.add_rect(v11, v3, v5, v6);
            mesh.add_tri(v1, v3, v5);
            break;
          
          case 106:
            mesh.add_tri(v3, v4, v12);
            mesh.add_rect(v2, v1, v7, v11);
            mesh.add_tri(v1, v5, v7);
            break;

          case 154:
            mesh.add_tri(v1, v2, v10);
            mesh.add_rect(v4, v3, v5, v9);
            mesh.add_tri(v3, v5, v7);
            break;

          case 169:
            mesh.add_tri(v5, v6, v10);
            mesh.add_rect(v9, v1, v7, v8);
            mesh.add_tri(v1, v3, v7);
            break;

          case 89:
            mesh.add_tri(v6, v7, v11);
            mesh.add_rect(v8, v5, v3, v12);
            mesh.add_tri(v1, v3, v5);
            break;

          case 149:
            mesh.add_tri(v2, v3, v11);
            mesh.add_rect(v12, v7, v1, v4);
            mesh.add_tri(v1, v5, v7);
            break;

          case 197:
            mesh.add_tri(v1, v4, v9);
            mesh.add_rect(v3, v8, v5, v2);
            mesh.add_tri(v3, v8, v12);
            break;

          case 92:
            mesh.add_tri(v5, v8, v9);
            mesh.add_rect(v4, v12, v6, v2);
            mesh.add_tri(v6, v7, v12);
            break;

          case 172:
            mesh.add_tri(v5, v6, v10);
            mesh.add_rect(v7, v8, v2, v11);
            mesh.add_tri(v2, v4, v8);
            break;

          case 202:
            mesh.add_tri(v1, v2, v10);
            mesh.add_rect(v11, v6, v4, v3);
            mesh.add_tri(v4, v6, v8);
            break;

          case 58:
            mesh.add_tri(v3, v4, v12);
            mesh.add_rect(v9, v8, v1, v2);
            mesh.add_tri(v2, v6, v8);
            break;

          case 163:
            mesh.add_tri(v7, v8, v12);
            mesh.add_rect(v5, v6, v4, v9);
            mesh.add_tri(v2, v4, v6);
            break;

          case 83:
            mesh.add_tri(v6, v7, v11);
            mesh.add_rect(v10, v2, v8, v5);
            mesh.add_tri(v2, v4, v8);
            break;

          case 53:
            mesh.add_tri(v2, v3, v11);
            mesh.add_rect(v1, v4, v6, v10);
            mesh.add_tri(v4, v6, v8);
            break;
          //------
          
          //------
          // CASE7:
          case 90:
            mesh.add_tri(v5, v8, v9);
            mesh.add_tri(v6, v7, v11);
            mesh.add_tri(v1, v2, v10);
            mesh.add_tri(v3, v4, v12);
            break;

          case 165:
            mesh.add_tri(v5, v6, v10);
            mesh.add_tri(v7, v8, v12);
            mesh.add_tri(v1, v4, v9);
            mesh.add_tri(v2, v3, v11);
            break;
          //------
          
          
          
          //------
          // CASE8:
          case 27:
            mesh.add_tri(v5, v8, v12);
            mesh.add_rect(v12, v5, v10, v3);
            mesh.add_tri(v2, v3, v10);
            break;

          case 39:
            mesh.add_tri(v4, v5, v9);
            mesh.add_rect(v4, v5, v6, v3);
            mesh.add_tri(v3, v6, v11);
            break;

          case 78:
            mesh.add_tri(v4, v7, v12);
            mesh.add_rect(v4, v7, v6, v1);
            mesh.add_tri(v1, v6, v10);
            break;

          case 141:
            mesh.add_tri(v7, v8, v9);
            mesh.add_rect(v9, v7, v11, v1);
            mesh.add_tri(v1, v2, v11);
            break;

          case 177:
            mesh.add_tri(v6, v7, v10);
            mesh.add_rect(v4, v12, v7, v10);
            mesh.add_tri(v1, v4, v10);
            break;

          case 114:
            mesh.add_tri(v7, v8, v9);
            mesh.add_rect(v9, v7, v11, v1);
            mesh.add_tri(v1, v2, v11);
            break;

          case 228:
            mesh.add_tri(v5, v8, v12);
            mesh.add_tri(v5, v10, v12);
            mesh.add_tri(v3, v10, v12);
            mesh.add_tri(v2, v3, v10);
            break;

          case 216:
            mesh.add_tri(v5, v6, v11);
            mesh.add_rect(v9, v5, v11, v3);
            mesh.add_tri(v3, v4, v9);
            break;
          //------
          
          

          //------
          // CASE9: &HACK&
          case 23:
            mesh.add_tri(v5, v8, v10);
            mesh.add_tri(v3, v4, v8);
            mesh.add_tri(v3, v10, v11);
            mesh.add_tri(v3, v8, v10);
            break;

          case 46:
            mesh.add_tri(v1, v5, v6);
            mesh.add_tri(v1, v6, v12);
            mesh.add_tri(v6, v11, v12);
            mesh.add_tri(v1, v4, v12);
            break;

          case 77:
            mesh.add_tri(v6, v7, v12);
            mesh.add_tri(v1, v2, v6);
            mesh.add_tri(v1, v9, v12);
            mesh.add_tri(v1, v6, v12);
            break;

          case 139:
            mesh.add_tri(v7, v8, v9);
            mesh.add_tri(v2, v3, v7);
            mesh.add_tri(v2, v9, v10);
            mesh.add_tri(v2, v7, v9);
            break;

          case 116:
            mesh.add_tri(v2, v3, v7);
            mesh.add_tri(v2, v7, v9);
            mesh.add_tri(v2, v9 ,v10);
            mesh.add_tri(v7, v8, v9);
            break;

          case 232:
            mesh.add_tri(v3, v10, v11);
            mesh.add_tri(v3, v8, v10);
            mesh.add_tri(v3, v4, v8);
            mesh.add_tri(v5, v8, v10);
            break;

          case 209:
            mesh.add_tri(v1, v4, v12);
            mesh.add_tri(v1, v5, v6);
            mesh.add_tri(v1, v6, v12);
            mesh.add_tri(v6, v11, v12);
            break;

          case 178:
            mesh.add_tri(v1, v9, v12);
            mesh.add_tri(v1, v6, v12);
            mesh.add_tri(v1, v2, v6);
            mesh.add_tri(v6, v7, v12);
            break;
          
          
          case 99:
            mesh.add_tri(v2, v4, v9);
            mesh.add_tri(v2, v7, v9);
            mesh.add_tri(v5, v7, v9);
            mesh.add_tri(v2, v7, v11);
            break;
          //case 46: PASS
          case 198:
            mesh.add_tri(v3, v8, v12);
            mesh.add_tri(v6, v8, v10);
            mesh.add_tri(v1, v3, v10);
            mesh.add_tri(v3, v8, v10);
            break;
          //case 116: PASS

          case 156:
            mesh.add_tri(v2, v7, v11);
            mesh.add_tri(v2, v4, v9);
            mesh.add_tri(v5, v7, v9);
            mesh.add_tri(v2, v7, v9);
            break;
          //case 209: PASS
          case 57:
            mesh.add_tri(v3, v8, v12);
            mesh.add_tri(v1, v3, v10);
            mesh.add_tri(v6, v8, v10);
            mesh.add_tri(v3, v8, v10);
            break;
          // case 139: PASS

          // case 198: PASS
          // case 77: PASS
          // case 153: PASS
          // case 232: PASS

          // case 178: PASS
          // case 99: PASS
          // case 23: PASS
          // case 57: PASS

          //------
          

          //------
          // CASE10:
          case 40:
            mesh.add_tri(v3, v4, v12);
            mesh.add_tri(v5, v6, v10);
            break;

          case 65:
            mesh.add_tri(v1, v4, v9);
            mesh.add_tri(v6, v7, v11);
            break;

          case 130:
            mesh.add_tri(v1, v2, v10);
            mesh.add_tri(v7, v8, v12);
            break;

          case 20:
            mesh.add_tri(v2, v3, v11);
            mesh.add_tri(v5, v8, v9);
            break;
          //------

          //------
          // CASE11:
          case 44:
            mesh.add_rect(v4, v12, v11, v2);
            mesh.add_tri(v5, v6, v10);
            break;

          case 73:
            mesh.add_rect(v1, v9, v12, v3);
            mesh.add_tri(v6, v7, v11);
            break;

          case 131:
            mesh.add_rect(v4, v9, v10, v2);
            mesh.add_tri(v7, v8, v12);
            break;

          case 22:
            mesh.add_rect(v3, v11, v10, v1);
            mesh.add_tri(v5, v8, v9);
            break;

          case 193:
            mesh.add_rect(v8, v12, v11, v6);
            mesh.add_tri(v1, v4, v9);
            break;

          case 146:
            mesh.add_rect(v12, v7, v5, v9);
            mesh.add_tri(v1, v2, v10);
            break;

          case 52:
            mesh.add_rect(v9, v8, v6, v10);
            mesh.add_tri(v2, v3, v11);
            break;

          case 104:
            mesh.add_rect(v11, v7, v5, v10);
            mesh.add_tri(v3, v4, v12);
            break;

          case 69:
            mesh.add_rect(v7, v6, v2, v3);
            mesh.add_tri(v1, v4, v9);
            break;

          case 138:
            mesh.add_rect(v8, v7, v3, v4);
            mesh.add_tri(v1, v2, v10);
            break;

          case 21:
            mesh.add_rect(v8, v5, v1, v4);
            mesh.add_tri(v2, v3, v11);
            break;

          case 42:
            mesh.add_rect(v5, v6, v2, v1);
            mesh.add_tri(v3, v4, v12);
            break;
          //------

          //------
          // CASE12:
          case 164:
            mesh.add_tri(v7, v8, v12);
            mesh.add_tri(v5, v6, v10);
            mesh.add_tri(v2, v3, v11);
            break;

          case 88:
            mesh.add_tri(v5, v8, v9);
            mesh.add_tri(v6, v7, v11);
            mesh.add_tri(v3, v4, v12);
            break;

          case 161:
            mesh.add_tri(v7, v8, v12);
            mesh.add_tri(v5, v6, v10);
            mesh.add_tri(v1, v4, v9);
            break;

          case 82:
            mesh.add_tri(v5, v8, v9);
            mesh.add_tri(v6, v7, v11);
            mesh.add_tri(v1, v2, v10);
            break;

          case 26:
            mesh.add_tri(v3, v4, v12);
            mesh.add_tri(v1, v2, v10);
            mesh.add_tri(v5, v8, v9);
            break;

          case 37:
            mesh.add_tri(v1, v4, v9);
            mesh.add_tri(v2, v3, v11);
            mesh.add_tri(v5, v6, v10);
            break;

          case 74:
            mesh.add_tri(v3, v4, v12);
            mesh.add_tri(v1, v2, v10);
            mesh.add_tri(v6, v7, v11);
            break;

          case 133:
            mesh.add_tri(v1, v4, v9);
            mesh.add_tri(v2, v3, v11);
            mesh.add_tri(v7, v8, v12);
            break;
          //------

          //------
          // CASE13:長方形2個
          case 170:
            mesh.add_rect(v8, v7, v3, v4);
            mesh.add_rect(v5, v6, v2, v1);
            break;

          case 85:
            mesh.add_rect(v7, v6, v2, v3);
            mesh.add_rect(v8, v5, v1, v4);
            break;

          case 105:
            mesh.add_rect(v7, v5, v10, v11);
            mesh.add_rect(v12, v9, v1, v3);
            break;

          case 150:
            mesh.add_rect(v7, v5, v9, v12);
            mesh.add_rect(v11, v10, v1, v3);
            break;

          case 195:
            mesh.add_rect(v12, v8, v6, v11);
            mesh.add_rect(v4, v9, v10, v2);
            break;

          case 60:
            mesh.add_rect(v9, v8, v6, v10);
            mesh.add_rect(v4, v12, v11, v2);
            break;
          //------

          //------
          // CASE14: &HACK&
          case 43:
            mesh.add_tri(v5, v6, v9);
            mesh.add_tri(v3, v9, v12);
            mesh.add_tri(v2, v3, v6);
            mesh.add_tri(v3, v6, v9);
            break;
          
          case 71:
            mesh.add_tri(v6, v9, v10);
            mesh.add_tri(v3, v6, v9);
            mesh.add_tri(v3, v6, v7);
            mesh.add_tri(v3, v4, v9);
            break;

          case 142:
            mesh.add_tri(v7, v8, v11);
            mesh.add_tri(v1, v4, v8);
            mesh.add_tri(v1, v10, v11);
            mesh.add_tri(v1, v8, v11);
            break;

          case 29:
            mesh.add_tri(v5, v8, v12);
            mesh.add_tri(v1, v2, v5);
            mesh.add_tri(v2, v11, v12);
            mesh.add_tri(v2, v5, v12);
            break;
          
          case 113:
            mesh.add_tri(v1, v4, v8);
            mesh.add_tri(v1, v8, v11);
            mesh.add_tri(v1, v10, v11);
            mesh.add_tri(v7, v8, v11);
            break;
          
          case 226:
            mesh.add_tri(v2, v11, v12);
            mesh.add_tri(v2, v5, v10);
            mesh.add_tri(v5, v8, v12);
            mesh.add_tri(v1, v2, v5);
            break;
          
          case 212:
            mesh.add_tri(v3, v6, v9);
            mesh.add_tri(v2, v3, v6);
            mesh.add_tri(v3, v9, v12);
            mesh.add_tri(v5, v6, v9);
            break;
          
          case 184:
            mesh.add_tri(v3, v4, v9);
            mesh.add_tri(v3, v6, v7);
            mesh.add_tri(v6, v9, v10);
            mesh.add_tri(v3, v6, v9);
            break;
          
          case 54:
            mesh.add_tri(v1, v3, v9);
            mesh.add_tri(v3, v6, v9);
            mesh.add_tri(v6, v8, v9);
            mesh.add_tri(v3, v6, v11);
            break;
          
          //case 71: PASS
          //case 98: PASS
          //case  226:

          case 147:
            mesh.add_tri(v5, v7, v10);
            mesh.add_tri(v4, v7, v10);
            mesh.add_tri(v2, v4, v10);
            mesh.add_tri(v4, v7, v12);
            break;
          // case 29: PASS
          // case 184: PASS
          case 201:
            mesh.add_tri(v3, v6, v11);
            mesh.add_tri(v6, v8, v9);
            mesh.add_tri(v1, v3, v9);
            mesh.add_tri(v3, v6, v9);
            break;
          
          case 108:
            mesh.add_tri(v5, v7, v10);
            mesh.add_tri(v4, v7, v10);
            mesh.add_tri(v2, v4, v10);
            mesh.add_tri(v4, v7, v12);
            break;
          // case 144: PASS
          // case 201: PASS
          // case 212: PASS

          // case 113: PASS
          // case 54: PASS
          // case 43: PASS
          // case 147: PASS
          //------

          //No case 
          //-------
          case 28:
            mesh.add_rect(v4, v12, v11, v2);
            mesh.add_tri(v5, v8, v9);
            break;
          
          case 31:
            mesh.add_tri(v5, v8, v12);
            mesh.add_tri(v5, v10, v12);
            mesh.add_tri(v10, v11, v12);
            break;
          
          case 41:
            mesh.add_tri(v1, v3, v11);
            mesh.add_tri(v1, v9, v12);
            mesh.add_tri(v5, v6, v10);
            break;
          
          case 47:
            mesh.add_tri(v5, v6, v9);
            mesh.add_tri(v6, v9, v10);
            mesh.add_tri(v9, v11, v12);
            break;
          
          case 55:
            mesh.add_tri(v3, v6, v11);
            mesh.add_tri(v3, v4, v6);
            mesh.add_tri(v4, v6, v8);
            break;
          
          case 56:
            mesh.add_tri(v6, v8, v10);
            mesh.add_tri(v8, v9, v10);
            mesh.add_tri(v3, v4, v12);
            break;
          
          case 59:
            mesh.add_tri(v2, v3, v12);
            mesh.add_tri(v2, v8, v12);
            mesh.add_tri(v2, v6, v8);
            break;
          
          case 61:
            mesh.add_tri(v1, v6, v8);
            mesh.add_tri(v1, v6, v10);
            mesh.add_tri(v1, v8, v12);
            mesh.add_tri(v1, v2, v11);
            mesh.add_tri(v1, v11, v12);
            break;
          
          case 62:
            mesh.add_tri(v1, v11, v12);
            mesh.add_tri(v1, v4, v12);
            mesh.add_tri(v1, v6, v12);
            mesh.add_tri(v1, v2, v11);
            mesh.add_tri(v1, v11, v12);
            break;
          
          case 63:
            mesh.add_tri(v6, v11, v12);
            mesh.add_tri(v6, v8, v12);
            break;
          
          case 67:
            mesh.add_tri(v2, v4, v9);
            mesh.add_tri(v2, v9, v10);
            mesh.add_tri(v6, v7, v11);
            break;
          
          case 79:
            mesh.add_tri(v6, v7, v10);
            mesh.add_tri(v7, v10, v12);
            mesh.add_tri(v9, v10, v12);
            break;
          
          case 81:
            mesh.add_tri(v1, v4, v5);
            mesh.add_tri(v4, v5, v8);
            mesh.add_tri(v6, v7, v11);
            break;
          
          case 84:
            mesh.add_tri(v2, v3, v7);
            mesh.add_tri(v2, v6, v7);
            mesh.add_tri(v5, v8, v9);
            break;
          
          case 87:
            mesh.add_tri(v4, v8, v10);
            mesh.add_tri(v5, v8, v10);
            mesh.add_tri(v3, v4, v10);
            mesh.add_tri(v6, v7, v10);
            mesh.add_tri(v3, v7, v10);
            break;
          
          case 91:
            mesh.add_tri(v2, v3, v10);
            mesh.add_tri(v3, v10, v12);
            mesh.add_tri(v5, v10, v12);
            mesh.add_tri(v5, v8, v12);
            mesh.add_tri(v6, v7, v11);
            break;

          case 93:
            mesh.add_tri(v2, v6, v12);
            mesh.add_tri(v6, v7, v12);
            mesh.add_tri(v1, v2, v12);
            mesh.add_tri(v5, v8, v12);
            mesh.add_tri(v1, v5, v12);
            break;
          
          case 94:
            mesh.add_tri(v1, v6, v10);
            mesh.add_tri(v1, v6, v7);
            mesh.add_tri(v1, v4, v7);
            mesh.add_tri(v4, v7, v12);
            mesh.add_tri(v5, v8, v9);
            break;

          case 95:
            mesh.add_tri(v6, v7, v10);
            mesh.add_tri(v7, v10, v12);
            mesh.add_tri(v5, v8, v10);
            mesh.add_tri(v8, v10, v12);
            break;
          
          case 97:
            mesh.add_rect(v11, v7, v5, v10);
            mesh.add_tri(v1, v4, v9);
            break;
          
          case 103:
            mesh.add_tri(v3, v4, v9);
            mesh.add_tri(v3, v5, v9);
            mesh.add_tri(v3, v5, v7);
            break;
          
          case 107:
            mesh.add_tri(v2, v5, v7);
            mesh.add_tri(v2, v7, v10);
            mesh.add_tri(v2, v5, v9);
            mesh.add_tri(v2, v3, v12);
            mesh.add_tri(v2, v9, v12);
            break;
          
          case 109:
            mesh.add_tri(v2, v9, v12);
            mesh.add_tri(v1, v2, v9);
            mesh.add_tri(v2, v7, v12);
            mesh.add_tri(v2, v5, v10);
            mesh.add_tri(v2, v5, v7);
            break;
          
          case 110:
            mesh.add_tri(v4, v7, v12);
            mesh.add_tri(v1, v4, v7);
            mesh.add_tri(v1, v5, v7);
            break;
          
          case 111:
            mesh.add_tri(v5, v7, v9);
            mesh.add_tri(v7, v9, v12);
            break;
          
          case 115:
            mesh.add_tri(v7, v8, v11);
            mesh.add_tri(v2, v8, v11);
            mesh.add_tri(v2, v4, v8);
            break;
          
          case 117:
            mesh.add_tri(v3, v7, v10);
            mesh.add_tri(v2, v3, v10);
            mesh.add_tri(v7, v8, v10);
            mesh.add_tri(v1, v4, v10);
            mesh.add_tri(v4, v8, v10);
            break;
          
          case 118:
            mesh.add_tri(v1, v8, v9);
            mesh.add_tri(v1, v7, v8);
            mesh.add_tri(v1, v3, v7);
            break;
          
          case 119:
            mesh.add_tri(v3, v4, v8);
            mesh.add_tri(v3, v7, v8);
            break;
          
          case 121:
            mesh.add_tri(v1, v3, v8);
            mesh.add_tri(v3, v8, v12);
            mesh.add_tri(v1, v8, v10);
            mesh.add_tri(v7, v8, v11);
            mesh.add_tri(v8, v10, v11);
            break;
          
          case 122:
            mesh.add_tri(v1, v2, v9);
            mesh.add_tri(v2, v8, v9);
            mesh.add_tri(v2, v8, v11);
            mesh.add_tri(v7, v8, v11);
            mesh.add_tri(v3, v4, v12);
            break;
          
          case 123:
            mesh.add_tri(v2, v3, v12);
            mesh.add_tri(v2, v8, v12);
            mesh.add_tri(v2, v7, v11);
            mesh.add_tri(v2, v7, v8);
            break;

          case 124:
            mesh.add_tri(v7, v9, v10);
            mesh.add_tri(v7, v8, v9);
            mesh.add_tri(v2, v7, v10);
            mesh.add_tri(v4, v7, v12);
            mesh.add_tri(v2, v4, v7);
            break;
          
          case 125:
            mesh.add_tri(v1, v2, v10);
            mesh.add_tri(v7, v8, v12);
            break;
          
          case 126:
            mesh.add_tri(v1, v8, v9);
            mesh.add_tri(v1, v7, v8);
            mesh.add_tri(v1, v4, v12);
            mesh.add_tri(v1, v7, v12);
            break;
          
          case 127:
            mesh.add_tri(v7, v8, v12);
            break;
          
          case 134:
            mesh.add_tri(v1, v3, v10);
            mesh.add_tri(v3, v10, v11);
            mesh.add_tri(v7, v8, v12);
            break;

          case 143:
            mesh.add_tri(v7, v8, v11);
            mesh.add_tri(v8, v9, v11);
            mesh.add_tri(v9, v10, v11);
            break;
          
          case 148:
            mesh.add_tri(v5, v7, v9);
            mesh.add_tri(v7, v9, v12);
            mesh.add_tri(v2, v3, v11);
            break;
          
          case 151:
            mesh.add_tri(v4, v10, v11);
            mesh.add_tri(v3, v4, v11);
            mesh.add_tri(v4, v5, v10);
            mesh.add_tri(v4, v7, v12);
            mesh.add_tri(v4, v5, v7);
            break;

          case 155:
            mesh.add_tri(v2, v5, v10);
            mesh.add_tri(v2, v3, v5);
            mesh.add_tri(v3, v5, v7);
            break;
          
          case 157:
            mesh.add_tri(v1, v2, v11);
            mesh.add_tri(v1, v7, v11);
            mesh.add_tri(v1, v5, v7);
            break;
          
          case 158:
            mesh.add_tri(v4, v5, v7);
            mesh.add_tri(v4, v5, v9);
            mesh.add_tri(v4, v7, v11);
            mesh.add_tri(v1, v4, v10);
            mesh.add_tri(v4, v10, v11);
            break;
          
          case 159:
            mesh.add_tri(v5, v10, v11);
            mesh.add_tri(v5, v7, v11);
            break;

          case 162:
            mesh.add_tri(v1, v2, v6);
            mesh.add_tri(v1, v5, v6);
            mesh.add_tri(v7, v8, v12);
            break;

          case 167:
            mesh.add_tri(v4, v5, v9);
            mesh.add_tri(v4, v5, v6);
            mesh.add_tri(v3, v4, v6);
            mesh.add_tri(v3, v6, v11);
            mesh.add_tri(v7, v8, v12);
            break;
          
          case 168:
            mesh.add_tri(v3, v4, v8);
            mesh.add_tri(v3, v7, v8);
            mesh.add_tri(v5, v6, v10);
            break;
          
          case 171:
            mesh.add_tri(v3, v7, v9);
            mesh.add_tri(v7, v8, v9);
            mesh.add_tri(v2, v3, v9);
            mesh.add_tri(v5, v6, v9);
            mesh.add_tri(v2, v6, v9);
            break;
          
          case 173:
            mesh.add_tri(v2, v7, v11);
            mesh.add_tri(v2, v7, v8);
            mesh.add_tri(v1, v2, v8);
            mesh.add_tri(v1, v8, v9);
            mesh.add_tri(v5, v6, v10);
            break;
          
          case 174:
            mesh.add_tri(v1, v5, v10);
            mesh.add_tri(v5, v6, v11);
            mesh.add_tri(v1, v4, v11);
            mesh.add_tri(v7, v8, v11);
            mesh.add_tri(v4, v8, v11);
            break;
          
          case 175:
            mesh.add_tri(v7, v8, v11);
            mesh.add_tri(v8, v9, v11);
            mesh.add_tri(v5, v6, v11);
            mesh.add_tri(v5, v9, v11);
            break;
          
          case 179:
            mesh.add_tri(v4, v7, v12);
            mesh.add_tri(v4, v6, v7);
            mesh.add_tri(v2, v4, v6); 
            break;
          
          case 181:
            mesh.add_tri(v1, v4, v12);
            mesh.add_tri(v1, v7, v12);
            mesh.add_tri(v1, v7, v10);
            mesh.add_tri(v6, v7, v10);
            mesh.add_tri(v2, v3, v11);
            break;
          
          case 182:
            mesh.add_tri(v6, v9, v12);
            mesh.add_tri(v6, v7, v12);
            mesh.add_tri(v1, v6, v9);
            mesh.add_tri(v3, v6, v11);
            mesh.add_tri(v1, v3, v6);
            break;
          
          case 183:
            mesh.add_tri(v4, v7, v12);
            mesh.add_tri(v4, v6, v7);
            mesh.add_tri(v3, v4, v11);
            mesh.add_tri(v4, v6, v11);
            break;
          
          case 185:
            mesh.add_tri(v6, v7, v10);
            mesh.add_tri(v1, v7, v10);
            mesh.add_tri(v1, v3, v7);
            break;

          case 186:
            mesh.add_tri(v2, v6, v9);
            mesh.add_tri(v1, v2, v9);
            mesh.add_tri(v6, v7, v9);
            mesh.add_tri(v3, v4, v9);
            mesh.add_tri(v3, v7, v9);
            break;
          
          case 187:
            mesh.add_tri(v2, v6, v7);
            mesh.add_tri(v2, v3, v7);
            break;
          
          case 188:
            mesh.add_tri(v2, v4, v7);
            mesh.add_tri(v2, v7, v11);
            mesh.add_tri(v4, v7, v9);
            mesh.add_tri(v6, v7, v10);
            mesh.add_tri(v7, v9, v10);
            break;
          
          case 189:
            mesh.add_tri(v1, v2, v11);
            mesh.add_tri(v1, v7, v11);
            mesh.add_tri(v1, v6, v10);
            mesh.add_tri(v1, v6, v7);
            break;
          
          case 190:
            mesh.add_tri(v1, v4, v9);
            mesh.add_tri(v6, v7, v11);
            break;
          
          case 191:
            mesh.add_tri(v6, v7, v11);
            break;
          
          case 194:
            mesh.add_tri(v6, v8, v12);
            mesh.add_tri(v6, v11, v12);
            mesh.add_tri(v1, v2, v10);
            break;
          
          case 199:
            mesh.add_tri(v3, v6, v8);
            mesh.add_tri(v3, v8, v12);
            mesh.add_tri(v3, v6, v10);
            mesh.add_tri(v3, v4, v9);
            mesh.add_tri(v3, v9, v10);
            break;
          
          case 203:
            mesh.add_tri(v3, v9, v10);
            mesh.add_tri(v2, v3, v10);
            mesh.add_tri(v3, v8, v9);
            mesh.add_tri(v3, v6, v11);
            mesh.add_tri(v3, v6, v8);
            break;
          
          case 205:
            mesh.add_tri(v1, v8, v9);
            mesh.add_tri(v1, v2, v8);
            mesh.add_tri(v2, v6, v8);
            break;
          
          case 206:
            mesh.add_tri(v1, v4, v10);
            mesh.add_tri(v4, v6, v10);
            mesh.add_tri(v4, v6, v8);
            break;
          
          case 207:
            mesh.add_rect(v9, v8, v6, v10);
            break;
          
          case 211:
            mesh.add_tri(v5, v11, v12);
            mesh.add_tri(v5, v6, v11);
            mesh.add_tri(v4, v5, v12);
            mesh.add_tri(v2, v5, v10);
            mesh.add_tri(v2, v4, v5);
            break;
          
          case 213:
            mesh.add_tri(v1, v5, v12);
            mesh.add_tri(v1, v4, v12);
            mesh.add_tri(v5, v6, v12);
            mesh.add_tri(v2, v3, v12);
            mesh.add_tri(v2, v6, v12);
            break;
          
          case 214:
            mesh.add_tri(v1, v3, v6);
            mesh.add_tri(v1, v6, v10);
            mesh.add_tri(v3, v6, v12);
            mesh.add_tri(v5, v6, v9);
            mesh.add_tri(v6, v9, v12);
            break;
          
          case 215:
            mesh.add_tri(v5, v6, v10);
            mesh.add_tri(v3, v4, v12);
            break;
          
          case 217:
            mesh.add_tri(v3, v6, v11);
            mesh.add_tri(v3, v5, v6);
            mesh.add_tri(v1, v3, v5);
            break;
          
          case 218:
            mesh.add_tri(v3, v4, v11);
            mesh.add_tri(v4, v6, v11);
            mesh.add_tri(v4, v6, v9);
            mesh.add_tri(v5, v6, v9);
            mesh.add_tri(v1, v2, v10);
            break;
          
          case 219:
            mesh.add_tri(v3, v6, v11);
            mesh.add_tri(v3, v5, v6);
            mesh.add_tri(v2, v3, v10);
            mesh.add_tri(v3, v5, v10);
            break;
          
          case 220:
            mesh.add_tri(v5, v6, v9);
            mesh.add_tri(v4, v6, v9);
            mesh.add_tri(v2, v4, v6);
            break;
          
          case 221:
            mesh.add_tri(v1, v5, v6);
            mesh.add_tri(v1, v2, v6);
            break;
          
          case 222:
            mesh.add_tri(v5, v6, v9);
            mesh.add_tri(v4, v6, v9);
            mesh.add_tri(v1, v6, v10);
            mesh.add_tri(v1, v4, v6);
            break;
          
          case 223:
            mesh.add_tri(v5, v6, v10);
            break;
          
          case 227:
            mesh.add_tri(v2, v4, v5);
            mesh.add_tri(v4, v5, v9);
            mesh.add_tri(v2, v5, v11);
            mesh.add_tri(v5, v8, v12);
            mesh.add_tri(v5, v11, v12);
            break;
          
          case 229:
            mesh.add_tri(v5, v8, v10);
            mesh.add_tri(v8, v10, v12);
            mesh.add_tri(v2, v10, v12);
            mesh.add_tri(v2, v3, v12);
            mesh.add_tri(v1, v4, v9);
            break;
          
          case 230:
            mesh.add_tri(v5, v8, v12);
            mesh.add_tri(v3, v5, v12);
            mesh.add_tri(v1, v3, v5);
            break;
          
          case 231:
            mesh.add_tri(v5, v8, v12);
            mesh.add_tri(v3, v5, v12);
            mesh.add_tri(v4, v5, v9);
            mesh.add_tri(v3, v4, v5);
            break;
          
          case 233:
            mesh.add_tri(v8, v10, v11);
            mesh.add_tri(v5, v8, v10);
            mesh.add_tri(v3, v8, v11);
            mesh.add_tri(v1, v8, v9);
            mesh.add_tri(v1, v3, v8);
            break;
          
          case 234:
            mesh.add_tri(v4, v8, v11);
            mesh.add_tri(v3, v4, v11);
            mesh.add_tri(v5, v8, v11);
            mesh.add_tri(v1, v2, v11);
            mesh.add_tri(v1, v5, v11);
            break;
          
          case 235:
            mesh.add_tri(v2, v3, v11);
            mesh.add_tri(v5, v8, v9);
            break;
          
          case 236:
            mesh.add_tri(v2, v5, v10);
            mesh.add_tri(v2, v5, v8);
            mesh.add_tri(v2, v4, v8);
            break;
          
          case 237:
            mesh.add_tri(v2, v5, v10);
            mesh.add_tri(v2, v5, v8);
            mesh.add_tri(v1, v2, v9);
            mesh.add_tri(v2, v8, v9);
            break;
          
          case 238:
            mesh.add_tri(v1, v4, v5);
            mesh.add_tri(v4, v5, v8);
            break;
          
          case 239:
            mesh.add_tri(v5, v8, v9);
            break;
          
          case 241:
            mesh.add_tri(v1, v4, v10);
            mesh.add_tri(v4, v10, v12);
            mesh.add_tri(v10, v11, v12);
            break;
          
          case 242:
            mesh.add_tri(v1, v2, v11);
            mesh.add_tri(v1, v9, v11);
            mesh.add_tri(v9, v11, v12);
            break;
          
          case 243:
            mesh.add_tri(v2, v4, v11);
            mesh.add_tri(v4, v11, v12);
            break;
          
          case 244:
            mesh.add_tri(v2, v3, v12);
            mesh.add_tri(v2, v10, v12);
            mesh.add_tri(v9, v10, v12);
            break;
          
          case 245:
            mesh.add_tri(v1, v4, v10);
            mesh.add_tri(v4, v10, v12);
            mesh.add_tri(v2, v3, v10);
            mesh.add_tri(v3, v10, v12);
            break;
          
          case 246:
            mesh.add_tri(v1, v3, v12);
            mesh.add_tri(v1, v9, v12);
            break;
          
          case 247:
            mesh.add_tri(v3, v4, v12);
            break;
          
          case 248:
            mesh.add_tri(v3, v4, v9);
            mesh.add_tri(v3, v9, v11);
            mesh.add_tri(v9, v10, v11);
            break;
          
          case 249:
            mesh.add_tri(v3, v10, v11);
            mesh.add_tri(v1, v3, v10);
            break;
          
          case 250:
            mesh.add_tri(v3, v4, v9);
            mesh.add_tri(v3, v9, v11);
            mesh.add_tri(v1, v2, v9);
            mesh.add_tri(v2, v9, v11);
            break;
          
          case 251:
            mesh.add_tri(v2, v3, v11);
            break;
          
          case 252:
            mesh.add_tri(v2, v4, v9);
            mesh.add_tri(v2, v9, v10);
            break;
          
          case 253:
            mesh.add_tri(v1, v2, v10);
            break;
          
          case 254:
            mesh.add_tri(v1, v4, v9);
            break;
          }
        }
      }
    }
  }

private:
  double calc_t(double const v1, double const v2) const noexcept
  {
    return v1 / (v1 + v2);
  }

  double lerp(double const v1, double const v2, double const t) const noexcept
  {
    return v1 * t + v2 * (1 - t);
  }
};
