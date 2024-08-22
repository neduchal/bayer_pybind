#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

// Bilinear algorithm reworked from https://github.com/jdthomas/bayer2rgb/blob/master/bayer.c
void bayerBilinearUINT32(const uint32_t* bayer, uint32_t* rgb, int sx, int sy, std::string tile = "BG")
{
	int bayerStep = sx;
	int rgbStep = 3 * sx;
	int width = sx;
	int height = sy;


	// From https://github.com/jdthomas/bayer2rgb/blob/master/bayer.c Bayer tile decision:
	//
	// int blue = tile == DC1394_COLOR_FILTER_BGGR || tile == DC1394_COLOR_FILTER_GBRG ? -1 : 1;
	// int start_with_green = tile == DC1394_COLOR_FILTER_GBRG || tile == DC1394_COLOR_FILTER_GRBG;
	//
	// Assume Bayer tile pattern is RGGB:
	int blue = -1;
	int start_with_green = 0;

    // 	int blue = tile == "BG" || tile == "GB" ? -1 : 1;
    // 	int start_with_green = tile == "GB" || tile == "GR";
	rgb += rgbStep + 3 + 1;
	height -= 2;
	width -= 2;

	for (; height--; bayer += bayerStep, rgb += rgbStep) {
		int t0, t1;
		const uint32_t *bayerEnd = bayer + width;

		if (start_with_green) {
			// OpenCV has a bug in the next line, which was
            // t0 = (bayer[0] + bayer[bayerStep * 2] + 1) >> 1;
			t0 = (bayer[1] + bayer[bayerStep * 2 + 1] + 1) >> 1;
			t1 = (bayer[bayerStep] + bayer[bayerStep + 2] + 1) >> 1;
			rgb[-blue] = (uint32_t)t0;
			rgb[0] = bayer[bayerStep + 1];
			rgb[blue] = (uint32_t)t1;
			bayer++;
			rgb += 3;
		}

		if (blue > 0) {
			for (; bayer <= bayerEnd - 2; bayer += 2, rgb += 6) {
				t0 = (bayer[0] + bayer[2] + bayer[bayerStep * 2] +
					bayer[bayerStep * 2 + 2] + 2) >> 2;
				t1 = (bayer[1] + bayer[bayerStep] +
					bayer[bayerStep + 2] + bayer[bayerStep * 2 + 1] +
					2) >> 2;
				rgb[-1] = (uint32_t)t0;
				rgb[0] = (uint32_t)t1;
				rgb[1] = bayer[bayerStep + 1];

				t0 = (bayer[2] + bayer[bayerStep * 2 + 2] + 1) >> 1;
				t1 = (bayer[bayerStep + 1] + bayer[bayerStep + 3] +
					1) >> 1;
				rgb[2] = (uint32_t)t0;
				rgb[3] = bayer[bayerStep + 2];
				rgb[4] = (uint32_t)t1;
			}
		}
		else {
			for (; bayer <= bayerEnd - 2; bayer += 2, rgb += 6) {
				t0 = (bayer[0] + bayer[2] + bayer[bayerStep * 2] +
					bayer[bayerStep * 2 + 2] + 2) >> 2;
				t1 = (bayer[1] + bayer[bayerStep] +
					bayer[bayerStep + 2] + bayer[bayerStep * 2 + 1] +
					2) >> 2;
				rgb[1] = (uint32_t)t0;
				rgb[0] = (uint32_t)t1;
				rgb[-1] = bayer[bayerStep + 1];

				t0 = (bayer[2] + bayer[bayerStep * 2 + 2] + 1) >> 1;
				t1 = (bayer[bayerStep + 1] + bayer[bayerStep + 3] +
					1) >> 1;
				rgb[4] = (uint32_t)t0;
				rgb[3] = bayer[bayerStep + 2];
				rgb[2] = (uint32_t)t1;
			}
		}

		if (bayer < bayerEnd) {
			t0 = (bayer[0] + bayer[2] + bayer[bayerStep * 2] +
				bayer[bayerStep * 2 + 2] + 2) >> 2;
			t1 = (bayer[1] + bayer[bayerStep] +
				bayer[bayerStep + 2] + bayer[bayerStep * 2 + 1] +
				2) >> 2;
			rgb[-blue] = (uint32_t)t0;
			rgb[0] = (uint32_t)t1;
			rgb[blue] = bayer[bayerStep + 1];
			bayer++;
			rgb += 3;
		}

		bayer -= width;
		rgb -= width * 3;

		blue = -blue;
		start_with_green = !start_with_green;
	}
}

PYBIND11_MODULE(bayer, m) {
    m.def("bayerBilinearUINT32", &bayer_Bilinear_uint32, "Bilinear Bayer to RGB conversion",
          pybind11::arg("bayer"), pybind11::arg("rgb"), pybind11::arg("sx"), pybind11::arg("sy"),
          pybind11::arg("tile") = "BG");
}