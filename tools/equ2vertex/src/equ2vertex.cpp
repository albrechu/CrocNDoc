#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>
#include <string>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <vector>
#include <unordered_map>
#include <unordered_set>

template <typename T> 
int sign(T val) {
	return (T(0) < val) - (val < T(0));
}

typedef struct v2i
{
	int y, x;
} v2i;

typedef struct v3f
{
	float x, y, z;

	v3f() : x(0), y(0), z(0) {}
	v3f(float fill)                : x(fill), y(fill), z(fill) { }
	v3f(float x, float y, float z) : x(x), y(y), z(z) { }
} v3f;

typedef struct v2f
{
	float x, y;

	v2f() : x(0), y(0) {}
	v2f(float fill) : x(fill), y(fill) {}
	v2f(float x, float y) : x(x), y(y) {}

	bool operator==(const v2f& other) const 
	{
        return std::abs(x - other.x) < 1e-1f && std::abs(y - other.y) < 1e-1f;
    }

	v2f& operator -=(const v2f& other)
	{
		x -= other.x;
		y -= other.y;
		return *this;
	}
} v2f;

template <>
struct std::hash<v2f>
{
	std::size_t operator()(v2f const& k) const
	{
		return std::hash<int>{}(std::round(k.x * 10000.0f)) ^ (std::hash<int>{}(std::round(k.y * 10000.0f)) << 1);
	}
};

typedef struct line_t
{
	v2f start, stop;

	line_t(v2f start, v2f stop) : start(start), stop(stop) {}

	bool operator==(const line_t& other) const
	{
		return (start == other.start && stop == other.stop) || (stop == other.start && start == other.stop);
	}
} line_t;

template <>
struct std::hash<line_t>
{
	std::size_t operator()(line_t const& k) const
	{
		return std::hash<v2f>{}(k.start) ^ std::hash<v2f>{}(k.stop);
	}
};

v2i fermat(float a, float phi, float omega, float t)
{
	return v2i{ (int)(a * sqrt(phi) * sin(phi + omega * t)), (int)(a * sqrt(phi) * cos(phi + omega * t)) };
}

int gerstner(float A, float x, float lambda, float T, float t)
{
	return (int)(A * sin((2 * M_PI / lambda) * x - (2 * M_PI / T) * t));
}

void fermat2lines(std::string const& name, int samples, int points, std::string& o)
{
	// Params
	float a     = 8;
	float omega = (2 * M_PI);
	float dphi  = omega / points;
	float dt    = omega / (samples - 0.5);
	// Add header
	o += "const i8 const " + name + "[" + std::to_string(samples) + "][" + std::to_string(points * 2 + 1) + "] = \n{\n";
	// Iterations
	float phi  = 0, t = 0;
	v2i   last = v2i{ 0, 0 };
	for (int s = 0; s < samples; ++s, t += dt)
	{
		o += "\t{\n\t\tLENGTH(" + std::to_string(points) + "),\n";
		phi  = dphi;
		last = fermat(a, 0, omega, t);
		for (int p = 0; p < points; ++p, phi += dphi)
		{
			v2i current = fermat(a, phi, omega, t);
			o += "\t\t" + std::to_string(current.y - last.y) + ", " + std::to_string(current.x - last.x) + ",\n";
			last = current;
		}
		phi = 0;
		o += "\t},\n";
	}
	o += "};\n";
}

void gerstner2lines(std::string const& name, int samples, int points, std::string& o)
{
	// Parameterrs
	float A      = 4;
	float lambda = 32;
	float dx     = lambda / points;
	float dt     = (2.0f * M_PI) / samples;
	float T      = 0.5;
	// Add Header
	o += "const i8 const " + name + "[" + std::to_string(samples) + "][" + std::to_string(points * 2 + 1) + "] =\n{\n";
	// Iterations
	float x    = 0, t = 0;
	v2i   last = v2i{ 0, 0 };
	for (int s = 0; s < samples; ++s, t += dt)
	{
		o += "\t{\n\t\tLENGTH(" + std::to_string(points) + "),\n";
		x = dx;
		last.y = gerstner(A, 0, lambda, T, t);
		last.x = 0;

		for (int p = 0; p < points; ++p, x += dx)
		{
			v2i current;
			current.y = gerstner(A, x, lambda, T, t);
			current.x = x;
			o += "\t\t" + std::to_string(current.y - last.y) + ", " + std::to_string(current.x - last.x) + ",\n";
			last = current;
		}
		o += "\t},\n";
		x   = 0;
	}
	o += "};\n";
}

float remap(float x, float a, float b, float c, float d)
{
	return (((x - a) / (b - a)) * (d - c)) + c;
}

v3f floor(v3f const& x)
{
	return { (float)(floor(x.x)), (float)(floor(x.y)) , (float)(floor(x.z)) };
}

v3f fract(v3f const& x)
{
	return { (float)(x.x - floor(x.x)), (float)(x.y - floor(x.y)) , (float)(x.z - floor(x.z)) };
}

v2f operator - (v2f const& x, v2f const& y) { return { x.x - y.x, x.y - y.y }; }
v2f componentMul(v2f const& x, v2f const& y) { return { x.x * y.x, x.y * y.y }; }
float length(v2f const& x) { return std::sqrt(x.x * x.x + x.y * x.y); }
v2f abs(v2f const& x) { return { abs(x.x), abs(x.y) }; }

v3f operator + (v3f const& x, v3f const& y) { return { x.x + y.x, x.y + y.y, x.z + y.z }; }
v3f operator - (v3f const& x, v3f const& y) { return { x.x - y.x, x.y - y.y, x.z - y.z }; }
v3f operator * (float const& a, v3f const& x) { return { a * x.x, a * x.y, a * x.z }; }
v3f operator * (v3f const& x, float const& a) { return { a * x.x, a * x.y, a * x.z }; }
v3f componentMul(v3f const& x, v3f const& y) { return { x.x * y.x, x.y * y.y, x.z * y.z }; }
v3f componentDiv(v3f const& x, v3f const& y) { return { x.x * y.x, x.y * y.y, x.z * y.z }; }
float dot(v3f const& x, v3f const& y) { return x.x * y.x + x.y * y.y + x.z * y.z; }

v3f hash33(v3f p)
{
	constexpr float uif = (1.0 / float(0xffffffffU));
	constexpr uint32_t ui0 = 1597334673U, ui1 = 3812015801U, ui2 = 2798796415U;
	p = floor(p);
	uint32_t q = (ui0 * ((uint32_t)((int32_t)p.x))) ^ (ui1 * ((uint32_t)((int32_t)p.y))) ^ (ui2 * ((uint32_t)((int32_t)p.z)));
	return -1. + 2. * v3f(q * ui0, q * ui1, q * ui2) * uif;
}

v3f mod(v3f x, v3f y)
{
	return x - componentMul(y, floor(componentDiv(x, y)));
}

float gradientNoise(v3f x, float f)
{
	// grid
	v3f p = floor(x);
	v3f w = fract(x);

	// quintic interpolant
	v3f u = componentMul(componentMul(componentMul(w, w), w), (componentMul(w, (w * 6. - 15.)) + 10.));

	// gradients
	v3f ga = hash33(mod(p + v3f(0., 0., 0.), f));
	v3f gb = hash33(mod(p + v3f(1., 0., 0.), f));
	v3f gc = hash33(mod(p + v3f(0., 1., 0.), f));
	v3f gd = hash33(mod(p + v3f(1., 1., 0.), f));
	v3f ge = hash33(mod(p + v3f(0., 0., 1.), f));
	v3f gf = hash33(mod(p + v3f(1., 0., 1.), f));
	v3f gg = hash33(mod(p + v3f(0., 1., 1.), f));
	v3f gh = hash33(mod(p + v3f(1., 1., 1.), f));

	// projections
	float va = dot(ga, w - v3f(0., 0., 0.));
	float vb = dot(gb, w - v3f(1., 0., 0.));
	float vc = dot(gc, w - v3f(0., 1., 0.));
	float vd = dot(gd, w - v3f(1., 1., 0.));
	float ve = dot(ge, w - v3f(0., 0., 1.));
	float vf = dot(gf, w - v3f(1., 0., 1.));
	float vg = dot(gg, w - v3f(0., 1., 1.));
	float vh = dot(gh, w - v3f(1., 1., 1.));

	// interpolation
	return va +
		u.x * (vb - va) +
		u.y * (vc - va) +
		u.z * (ve - va) +
		u.x * u.y * (va - vb - vc + vd) +
		u.y * u.z * (va - vc - ve + vg) +
		u.z * u.x * (va - vb - ve + vf) +
		u.x * u.y * u.z * (-va + vb + vc - vd + ve - vf - vg + vh);
}


float worley(v3f uv, float freq)
{
	v3f id = floor(uv);
	v3f p  = fract(uv);

	float minDist = 10000.;
	for (float x = -1.; x <= 1.; ++x)
	{
		for (float y = -1.; y <= 1.; ++y)
		{
			for (float z = -1.; z <= 1.; ++z)
			{
				v3f offset = v3f{ x, y, z };
				v3f h = hash33(mod(id + offset, v3f(freq))) * .5 + .5;
				h = h + offset;
				v3f d = p - h;
				minDist = min(minDist, dot(d, d));
			}
		}
	}
	// inverted worley noise
	return 1. - minDist;
}

float perlinfbm(v3f p, float freq, int octaves)
{
	float G = exp2(-.85);
	float amp = 1.;
	float noise = 0.;
	for (int i = 0; i < octaves; ++i)
	{
		noise += amp * gradientNoise(p * freq, freq);
		freq *= 2.;
		amp *= G;
	}
	return noise;
}

float worley_fractal_brownian_noise(v3f p, float f)
{
	return worley(p * f, f) * .625 + worley(p * f * 2., f * 2.) * .25 + worley(p * f * 4., f * 4.) * .125;
}

enum Edge_ { Edge_Top, Edge_Right, Edge_Bottom, Edge_Left };
typedef int Edge;

constexpr static Edge g_lines[][2] =
{
	{Edge_Left, Edge_Bottom},
	{Edge_Bottom, Edge_Right},
	{Edge_Left, Edge_Right},
	{Edge_Top, Edge_Right},
	{Edge_Top, Edge_Left},
	{Edge_Top, Edge_Bottom},
	{Edge_Left, Edge_Top},
	{Edge_Bottom, Edge_Top},
	{Edge_Right, Edge_Left},
	{Edge_Right, Edge_Bottom}
};

constexpr static int g_lineLUT[16][2] =
{
	{ -1, -1 },
	{  0, -1 },
	{  1, -1 },
	{  2, -1 },
	{  3, -1 },
	{  4,  1 },
	{  5, -1 },
	{  4, -1 },
	{  6, -1 },
	{  7, -1 },
	{  1,  6 },
	{  1, -1 },
	{  8, -1 },
	{  9, -1 },
	{  0, -1 },
	{ -1, -1 },
};

v2f interpolate(v2f p1, v2f p2, float v1, float v2, float threshold)
{
	float t = (threshold - v1) / (v2 - v1);
	return
	{
	   (p1.x + t * (p2.x - p1.x)),
	   (p1.y + t * (p2.y - p1.y)),
	};
};
v2f interpolate_edge(float x, float y, Edge edge, float tl, float tr, float br, float bl, float threshold)
{
	switch (edge)
	{
	case Edge_Top:    return interpolate({ x, y + 1 }, { x + 1, y + 1 }, tl, tr, threshold);
	case Edge_Right:  return interpolate({ x + 1, y + 1 }, { x + 1, y }, tr, br, threshold);
	case Edge_Bottom: return interpolate({ x + 1, y }, { x, y }, br, bl, threshold);
	case Edge_Left:   return interpolate({ x, y }, { x, y + 1 }, bl, tl, threshold);
	}
}

void marching_squares(std::vector<float> const& pixels, int width, int height, float threshold, std::vector<line_t>& out)
{
	out.clear();
	for (size_t y = 0; y < height - 1; y++)
	{
		for (size_t x = 0; x < width - 1; x++)
		{
			float tl = pixels[(y + 1) * width + x];
			float tr = pixels[(y + 1) * width + (x + 1)];
			float br = pixels[y * width + (x + 1)];
			float bl = pixels[y * width + x];

			int idx = (tl > threshold) * 8 |
				(tr > threshold) * 4 |
				(br > threshold) * 2 |
				(bl > threshold) * 1;
			const int* li = g_lineLUT[idx];
			for (int i = 0; i < 2; i++, li++)
			{
				if (*li >= 0)
				{
					out.emplace_back(
						interpolate_edge(x, y, g_lines[*li][0], tl, tr, br, bl, threshold),
						interpolate_edge(x, y, g_lines[*li][1], tl, tr, br, bl, threshold)
					);
				}
			}
		}
	}
}

void lines2polylines(std::vector<line_t> const& lines, std::vector<std::vector<v2f>>& polylines)
{
	std::unordered_map<v2f, std::vector<v2f>> adjacency;
	line_t const* l = (line_t const*)lines.data();
	for (size_t i = 0; i < (lines.size() >> 1); ++i, ++l)
	{
		adjacency[l->start].emplace_back(l->stop);
		adjacency[l->stop].emplace_back(l->start);
	}

	polylines.clear();
	std::unordered_set<line_t, std::hash<line_t>> visited;

	for (auto& entry : adjacency) {
		const v2f& start = entry.first;

		for (const v2f& neighbor : entry.second) {
			auto seg = line_t(start, neighbor);
			if (visited.count(seg)) continue;

			std::vector<v2f> polyline = { start, neighbor };
			visited.insert(seg);
			visited.insert({ neighbor, start });

			v2f current = neighbor;
			v2f previous = start;

			while (true) {
				const auto& nexts = adjacency[current];
				v2f next = {};
				bool found = false;

				for (const auto& candidate : nexts) {
					if (candidate.x == previous.x && candidate.y == previous.y) continue;
					if (!visited.count({ current, candidate })) {
						next = candidate;
						found = true;
						break;
					}
				}

				if (!found) break;

				polyline.push_back(next);
				visited.insert({ current, next });
				visited.insert({ next, current });

				previous = current;
				current = next;
			}
			polylines.push_back(polyline);
		}
	}
}

float sdEllipse(v2f p, v2f ab)
{
	p = abs(p); 
	
	if (p.x > p.y) 
	{ 
		std::swap(p.x, p.y);
		std::swap(ab.x, ab.y);
	}
	float l = ab.y * ab.y - ab.x * ab.x;
	float m = ab.x * p.x / l;      float m2 = m * m;
	float n = ab.y * p.y / l;      float n2 = n * n;
	float c = (m2 + n2 - 1.0) / 3.0; float c3 = c * c * c;
	float q = c3 + m2 * n2 * 2.0;
	float d = c3 + m2 * n2;
	float g = m + m * n2;
	float co;
	if (d < 0.0)
	{
		float h = acos(q / c3) / 3.0;
		float s = cos(h);
		float t = sin(h) * sqrt(3.0);
		float rx = sqrt(-c * (s + t + 2.0) + m2);
		float ry = sqrt(-c * (s - t + 2.0) + m2);
		co = (ry + sign(l) * rx + abs(g) / (rx * ry) - m) / 2.0;
	}
	else
	{
		float h = 2.0 * m * n * sqrt(d);
		float s = sign(q + h) * pow(abs(q + h), 1.0 / 3.0);
		float u = sign(q - h) * pow(abs(q - h), 1.0 / 3.0);
		float rx = -s - u - c * 4.0 + 2.0 * m2;
		float ry = (s - u) * sqrt(3.0);
		float rm = sqrt(rx * rx + ry * ry);
		co = (ry / sqrt(rm - rx) + 2.0 * g / rm - m) / 2.0;
	}
	v2f r = componentMul(ab,  v2f(co, sqrt(1.0 - co * co)));
	return length(r - p) * sign(p.y - r.y);
}

void wfbn2lines(std::string const& name, int samples, std::string& o)
{
	int width = 4, height = 4;
	float threshold = 0.8f;
	float f = 8;
	std::vector<float> tiles(width * height, 0);

	float dt = 1.f / samples;

	std::vector<std::vector<v2f>> strips(samples);
	float t = 0;

	for (size_t s = 0; s < samples; ++s, t += dt)
	{
		for (int y = 0, i = 0; y < height; ++y)
		{
			for (int x = 0; x < width; ++x)
			{
				v2f uv = { float(x) / width, float(y) / height };
				//uv -= v2f(0.5 * t);

				//tiles[i++] = worley_fractal_brownian_noise(v3f(uv.x, uv.y, 0.0), f);
				tiles[i++] += sdEllipse(uv, v2f(1.414 * width, 0.707 * height));
			}
		}

		std::vector<line_t> lines;
		marching_squares(tiles, width, height, threshold, lines);
		std::fill(tiles.begin(), tiles.end(), 0);
		std::vector<std::vector<v2f>> polylines;
		lines2polylines(lines, polylines);

		auto strip = std::max_element(polylines.begin(), polylines.end(), [](std::vector<v2f> const& a, std::vector<v2f> const& b) {
			return a.size() < b.size();
		});
		strips.emplace_back(std::move(*strip));
	}

	// Write sample strips to their each individual arrays
	int n = 0;
	v2i   last;
	for (std::vector<v2f>& s : strips)
	{
		last = v2i{ 0, 0 };
		// Add Header
		o += "const i8 " + name + std::to_string(n++) + "[" + std::to_string(s.size() * 2 + 1) + "] = \n{\nLENGTH(" + std::to_string(s.size()) + "),\n";
		for (int i = 0; i < s.size(); i++)
		{
			v2f currentF = s[i];
			v2i current = {  height * currentF.y, width * currentF.x };
			o += std::to_string(current.y - last.y) + ", " + std::to_string(current.x - last.x) + ", \n";
			last = current;
		}
		o += "};\n\n";
	}
	// Write the last array of pointers to each strip
	o += "const i8* const " + name + "[" + std::to_string(samples) + "] =\n{\n";
	for (int i = 0; i < n; ++i)
	{
		o += name + std::to_string(i) + ",\n";
	}
	o += "};\n";

}

v2f cloud_line(float phi, float t, float radius = 20.0f, float f = 2.0f, float jitter = 4.0f) {
	float noise = 2.5 * perlinfbm({ phi + t, 0, 0 }, f, 16);
	float rx = 0.25  * radius + noise;
	float ry = 0.225 * radius + noise;
	return { rx * cos(phi), ry * sin(phi) };
}

void cloud2lines(std::string const& name, int samples, int points, std::string& o)
{
	// Params
	float omega = (2 * M_PI);
	float dphi = omega / points;
	float dt = omega / samples;
	// Add header
	o += "const i8 const " + name + "[" + std::to_string(samples) + "][" + std::to_string(points * 2 + 1) + "] = \n{\n";
	// Iterations
	float phi = 0, t = 0;
	v2i   last = v2i{ 0, 0 };
	for (int s = 0; s < samples; ++s, t += dt)
	{
		o += "\t{\n\t\tLENGTH(" + std::to_string(points) + "),\n";
		phi = dphi;
		
		auto first = cloud_line(0, t);
		last = { (int)first.y, (int)first.x };
		for (int p = 0; p < points; ++p, phi += dphi)
		{
			v2f currentF = cloud_line(phi, t);
			v2i current = { currentF.y, currentF.x };
			o += "\t\t" + std::to_string(current.y - last.y) + ", " + std::to_string(current.x - last.x) + ",\n";
			last = current;
		}
		phi = 0;
		o += "\t},\n";
	}
	o += "};\n";
}

int main()
{
	
	int samples = 8;
	int points  = 4;

	v2i* vs = new v2i[points];
	std::string o = "";
	

	fermat2lines("spiral", samples, points, o); // Create Spiral
	//gerstner2lines("watertop", samples, points, o); // Create Waves
	//wfbn2lines("cloud", samples, o);
	//cloud2lines("cloud", samples, points, o);

	printf("%s\n Code also has been copied to clipboard. Note that this is a duffy type list!\n", o.c_str());
	// Copy to clipboard
	HGLOBAL mem = GlobalAlloc(GMEM_MOVEABLE, o.size() + 1);
	memcpy(GlobalLock(mem), o.c_str(), o.size() + 1);
	GlobalUnlock(mem);
	OpenClipboard(0);
	EmptyClipboard();
	SetClipboardData(CF_TEXT, mem);
	CloseClipboard();
}
