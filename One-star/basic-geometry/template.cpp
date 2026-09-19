/**
 * @file template.cpp
 * @brief Chuẩn thuật toán Hình học Tính toán Cơ bản (Basic Computational Geometry Framework)
 * @author Duc-Minh Vu (Vũ Đức Minh)
 * @affiliation SLSCM Lab - Faculty of Data Science and AI (FDA), National Economics University (NEU)
 * @generator Soạn thảo và tối ưu bởi Agentic AI tool
 * @copyright © 2026 Duc-Minh Vu. All rights reserved.
 * 
 * Chuẩn C++: C++20
 * Bộ công cụ bao gồm:
 * 1. Cấu trúc Điểm/Vector 2D (Point/Vector) số nguyên và số thực.
 * 2. Tích vô hướng (Dot) & Tích có hướng (Cross Product).
 * 3. Kiểm tra hướng rẽ CCW (Counter-Clockwise Test).
 * 4. Kiểm tra điểm thuộc đoạn thẳng và hai đoạn thẳng cắt nhau chuẩn xác.
 * 5. Giao điểm hai đường thẳng số thực.
 * 6. Công thức Dây giày (Shoelace) tính diện tích đa giác nguyên/thực.
 * 7. Kiểm tra vị trí tương đối Điểm với Đa giác (Point in Polygon).
 * 8. Định lý Pick đếm điểm nguyên trong đa giác lưới.
 */

#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <numeric>
#include <cassert>

using namespace std;

namespace Geometry {

    const double EPS = 1e-9;

    inline int sign(double x) {
        if (x > EPS) return 1;
        if (x < -EPS) return -1;
        return 0;
    }

    // ==========================================
    // 1. Cấu trúc Điểm & Vector 2D
    // ==========================================
    template <typename T = long long>
    struct Point {
        T x, y;
        Point(T x = 0, T y = 0) : x(x), y(y) {}

        Point operator+(const Point& o) const { return Point(x + o.x, y + o.y); }
        Point operator-(const Point& o) const { return Point(x - o.x, y - o.y); }
        Point operator*(T scalar) const { return Point(x * scalar, y * scalar); }
        Point operator/(T scalar) const { return Point(x / scalar, y / scalar); }

        bool operator==(const Point& o) const { return x == o.x && y == o.y; }
        bool operator!=(const Point& o) const { return !(*this == o); }
        bool operator<(const Point& o) const {
            if (x != o.x) return x < o.x;
            return y < o.y;
        }
    };

    using PointLL = Point<long long>;
    using PointD  = Point<double>;

    // ==========================================
    // 2. Tích Vô Hướng & Tích Có Hướng
    // ==========================================
    template <typename T>
    inline T dot(Point<T> a, Point<T> b) {
        return a.x * b.x + a.y * b.y;
    }

    template <typename T>
    inline T cross(Point<T> a, Point<T> b) {
        return a.x * b.y - a.y * b.x;
    }

    // Tích có hướng của hai vector AB và AC
    template <typename T>
    inline T cross(Point<T> a, Point<T> b, Point<T> c) {
        return cross(b - a, c - a);
    }

    // Bình phương khoảng cách giữa hai điểm
    template <typename T>
    inline T distSq(Point<T> a, Point<T> b) {
        T dx = a.x - b.x;
        T dy = a.y - b.y;
        return dx * dx + dy * dy;
    }

    // Khoảng cách Euclid giữa hai điểm
    template <typename T>
    inline double dist(Point<T> a, Point<T> b) {
        return sqrt((double)distSq(a, b));
    }

    // ==========================================
    // 3. Hướng Rẽ (Orientation / CCW)
    // ==========================================
    // 1: Rẽ trái (CCW), -1: Rẽ phải (CW), 0: Thẳng hàng (Collinear)
    template <typename T>
    inline int ccw(Point<T> a, Point<T> b, Point<T> c) {
        T cp = cross(a, b, c);
        if (cp > 0) return 1;
        if (cp < 0) return -1;
        return 0;
    }

    // ==========================================
    // 4. Đoạn thẳng & Giao điểm
    // ==========================================
    // Kiểm tra điểm p có nằm trên đoạn thẳng ab không (giả sử p thẳng hàng với ab)
    template <typename T>
    bool onSegment(Point<T> p, Point<T> a, Point<T> b) {
        if (cross(a, b, p) != 0) return false;
        return (p.x >= min(a.x, b.x) && p.x <= max(a.x, b.x) &&
                p.y >= min(a.y, b.y) && p.y <= max(a.y, b.y));
    }

    // Kiểm tra hai đoạn thẳng ab và cd có cắt nhau không (kể cả trường hợp suy biến)
    template <typename T>
    bool intersect(Point<T> a, Point<T> b, Point<T> c, Point<T> d) {
        int o1 = ccw(a, b, c);
        int o2 = ccw(a, b, d);
        int o3 = ccw(c, d, a);
        int o4 = ccw(c, d, b);

        // Trường hợp cắt thông thường
        if (o1 * o2 < 0 && o3 * o4 < 0) return true;

        // Trường hợp suy biến: đầu mút nằm trên đoạn thẳng kia
        if (o1 == 0 && onSegment(c, a, b)) return true;
        if (o2 == 0 && onSegment(d, a, b)) return true;
        if (o3 == 0 && onSegment(a, c, d)) return true;
        if (o4 == 0 && onSegment(b, c, d)) return true;

        return false;
    }

    // Giao điểm hai đường thẳng không song song
    inline PointD lineIntersection(PointD a, PointD b, PointD c, PointD d) {
        PointD ab = b - a;
        PointD cd = d - c;
        PointD ac = c - a;
        double cp = cross(ab, cd);
        assert(abs(cp) > EPS); // Đảm bảo hai đường thẳng không song song
        double t = cross(ac, cd) / cp;
        return a + ab * t;
    }

    // ==========================================
    // 5. Đa giác (Polygons)
    // ==========================================
    // Tính 2 * Diện tích có hướng của đa giác (chính xác số nguyên)
    template <typename T>
    T polygonArea2(const vector<Point<T>>& poly) {
        int n = poly.size();
        T sum = 0;
        for (int i = 0; i < n; ++i) {
            int j = (i + 1) % n;
            sum += cross(poly[i], poly[j]);
        }
        return abs(sum);
    }

    // Diện tích số thực của đa giác
    template <typename T>
    double polygonArea(const vector<Point<T>>& poly) {
        return (double)polygonArea2(poly) / 2.0;
    }

    // Vị trí tương đối của điểm q với đa giác đơn poly:
    // Trả về: 0: OUTSIDE, 1: BOUNDARY, 2: INSIDE
    template <typename T>
    int pointInPolygon(Point<T> q, const vector<Point<T>>& poly) {
        int n = poly.size();
        bool inside = false;

        for (int i = 0; i < n; ++i) {
            Point<T> a = poly[i];
            Point<T> b = poly[(i + 1) % n];

            if (onSegment(q, a, b)) return 1; // BOUNDARY

            if (a.y > b.y) swap(a, b);

            if (a.y <= q.y && q.y < b.y) {
                if (cross(a, b, q) > 0) {
                    inside = !inside;
                }
            }
        }

        return inside ? 2 : 0;
    }

    // ==========================================
    // 6. Định lý Pick trên Lưới Điểm Nguyên
    // ==========================================
    // Đếm số điểm nguyên trên biên đa giác
    long long countBoundaryPoints(const vector<PointLL>& poly) {
        int n = poly.size();
        long long b = 0;
        for (int i = 0; i < n; ++i) {
            int j = (i + 1) % n;
            long long dx = abs(poly[j].x - poly[i].x);
            long long dy = abs(poly[j].y - poly[i].y);
            b += std::gcd(dx, dy);
        }
        return b;
    }

    // Đếm số điểm nguyên nằm bên trong đa giác theo định lý Pick: I = Area - B/2 + 1
    long long countInteriorPoints(const vector<PointLL>& poly) {
        long long area2 = polygonArea2(poly);
        long long b = countBoundaryPoints(poly);
        // area2 = 2*S = 2*I + B - 2  =>  2*I = area2 - B + 2
        return (area2 - b + 2) / 2;
    }

} // namespace Geometry

// ==========================================
// Minh họa Tích hợp & Kiểm thử Độc lập
// ==========================================

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    using namespace Geometry;

    // 1. Kiểm thử Tích có hướng & CCW
    PointLL A(0, 0), B(4, 0), C(4, 3);
    assert(ccw(A, B, C) == 1);  // Rẽ trái (CCW)
    assert(ccw(C, B, A) == -1); // Rẽ phải (CW)
    assert(ccw(A, B, PointLL(8, 0)) == 0); // Thẳng hàng

    // 2. Kiểm thử Giao hai đoạn thẳng
    PointLL P1(0, 0), P2(4, 4);
    PointLL P3(0, 4), P4(4, 0);
    assert(intersect(P1, P2, P3, P4) == true);  // Cắt nhau tại (2, 2)
    assert(intersect(P1, P2, PointLL(5, 5), PointLL(6, 6)) == false); // Không giao

    // 3. Kiểm thử Diện tích đa giác & Định lý Pick trên hình vuông 4x4
    // Đỉnh: (0, 0) -> (4, 0) -> (4, 4) -> (0, 4)
    vector<PointLL> square = {
        PointLL(0, 0),
        PointLL(4, 0),
        PointLL(4, 4),
        PointLL(0, 4)
    };

    long long a2 = polygonArea2(square);
    assert(a2 == 32); // 2 * Area = 2 * 16 = 32
    cout << "Square 4x4 Area = " << polygonArea(square) << " (Expected: 16)\n";

    long long B_pts = countBoundaryPoints(square);
    long long I_pts = countInteriorPoints(square);
    assert(B_pts == 16); // 4 cạnh * 4 = 16 điểm biên
    assert(I_pts == 9);  // (4-1)*(4-1) = 9 điểm nguyên bên trong
    cout << "Boundary Points = " << B_pts << ", Interior Points = " << I_pts << "\n";

    // 4. Kiểm thử Point in Polygon
    assert(pointInPolygon(PointLL(2, 2), square) == 2); // INSIDE
    assert(pointInPolygon(PointLL(0, 2), square) == 1); // BOUNDARY
    assert(pointInPolygon(PointLL(5, 5), square) == 0); // OUTSIDE
    cout << "Point-in-Polygon tests passed!\n";

    cout << "\nAll Basic Geometry tests passed successfully!\n";
    return 0;
}
