#include <iostream>
#include <cmath>
#include <iomanip>

using namespace std;

double PI = 3.14159265358979323846;

int main()
{
    int numCircles = 20; // number of circles to generate
    double initialRadius = 5.0; // initial radius of the circles
    double radiusShrinkFactor = 1 / (numCircles + 0.5); // factor by which radius shrinks for each circle

    int numPoints = 10; // number of points to generate

    // loop through each circle
    for (int j = 0; j < numCircles; j++)
    {
        double angle1 = PI * (j + 0.5) / numCircles; // calculate the angle for latitude
        double radius1 = initialRadius * sin(angle1); // calculate the radius for latitude
        double z = initialRadius * cos(angle1); // calculate the z-coordinate

        // loop through and generate the coordinates of the points on the circle
        for (int i = 0; i < numPoints; i++)
        {
            double angle2 = (2.0 * PI * i) / numPoints; // calculate the angle for longitude
            double x = radius1 * cos(angle2); // calculate x-coordinate
            double y = radius1 * sin(angle2); // calculate y-coordinate

            // calculate the normal and tangent vectors
            double normx = x / initialRadius;
            double normy = y / initialRadius;
            double normz = z / initialRadius;
            double tanx = -sin(angle2);
            double tany = cos(angle2);
            double tanz = 0.0;
            double s = angle2 / (2 * PI);
            double t = angle1 / PI;

            // output the point coordinates, normal vector, and tangent vector
            cout << fixed << setprecision(2) << x << "f , " << y << "f , " << z << "f, "
                 << normx << "f, " << normy << "f, " << normz << "f, "
                 << tanx << "f, " << tany << "f, " << tanz << "f, " << s << "f, " << t << "f, " << endl;
        }

        cout << endl;
    }

    return 0;
}
