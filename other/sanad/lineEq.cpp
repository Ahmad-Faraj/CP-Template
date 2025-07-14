/* Line Eq */
vector <double> general_line_eq(double x1, double y1, double x2, double y2){
    // ax + by + c = 0 // This is the general eqaution
    double a = y1 - y2;
    double b = x2 - x1;
    double c = (x1 - x2) * y1 + (y2 - y1) * x1;
    //            -b     * y1 +     -a    * x1

    return {a, b, c};
}

// get the intersection of two points
pair <double, double> Intersection_of_two_lines(vector<double> Line1_4points, vector<double> Line2_4points){
    if (Line1_4points.size() != 4)  return cout << "Error Reading Line", make_pair(INT_MIN, INT_MIN);
    if (Line2_4points.size() != 4)  return cout << "Error Reading Line", make_pair(INT_MIN, INT_MIN);
    
    vector<double> l1 = general_line_eq(Line1_4points[0], Line1_4points[1], Line1_4points[2], Line1_4points[3]);
    vector<double> l2 = general_line_eq(Line2_4points[0], Line2_4points[1], Line2_4points[2], Line2_4points[3]);

    double a1 = l1[0], b1 = l1[1], c1 = l1[2];
    double a2 = l2[0], b2 = l2[1], c2 = l2[2];

/**
 *  We will get the point using cramer's rule:
 *  => l1 => a1 X + b1 Y + c1 = 0
 *  => l2 => a2 X + b2 Y + c2 = 0
 * 
 *  Let the intersecting point (Xo, Yo)
 *  Then:
 *      a1 Xo + b1 Yo + c1 = 0
 *      a2 Xo + b2 Yo + c2 = 0
 *      يعني نقطة التقاطع تحقق المعادلتين
 *      هنحل المعادلتين عن طريق كرامر 
 *  Xo / (b1 * c2 - b2 * c1) = -yo / (a1 * c2 - a2 * c1) = 1 / (a1 * b2 - a2 * b1)
 *  
 *  Xo = (b1 * c2 - b2 * c1) / (a1 * b2 - a2 * b1)
 *  Yo = -(a1 * c2 - a2 * c1) / (a1 * b2 - a2 * b1)
 */     

    double det = a1 * b2 - a2 * b1;

    if (!det)   // This means that the two lines are either parllel or same line (منطبقان يعني)
        return {DBL_MAX, DBL_MAX};

    return make_pair((b1 * c2 - b2 * c1) / det, (c1 * a2 - c2 * a1) / det);
}

// check if two lines have the same slope
bool same_slope(vector<double> Line1_4points, vector<double> Line2_4points){
    double x1 = Line1_4points[0], y1 = Line1_4points[1], x2 = Line1_4points[2], y2 = Line1_4points[3];
    double x3 = Line2_4points[0], y3 = Line2_4points[1], x4 = Line2_4points[2], y4 = Line2_4points[3];

    double delta_y_line1 = y1 - y2;
    double delta_x_line1 = x1 - x2;
    double delta_y_line2 = y3 - y4;
    double delta_x_line2 = x3 - x4;

    return delta_x_line1 * delta_y_line2 == delta_x_line2 * delta_y_line1;
}

// بعد ما احسب المحدد و انا بحاول اجيب نقطة التقاطع لو المحدد طلع بصفر ف كدة الخطين اما متوازيان او منطبقان
// ف الفانكشن دي بتشوف هل هم منطبقان ف بترجع ترو -- ولو هم متوازيان هترجع فولس
bool same_line_or_parllel(vector<double> Line1_4points, vector<double> Line2_4points){
    double x1 = Line1_4points[0], y1 = Line1_4points[1], x2 = Line1_4points[2], y2 = Line1_4points[3];
    double x3 = Line2_4points[0], y3 = Line2_4points[1], x4 = Line2_4points[2], y4 = Line2_4points[3];

    vector <double> line_eq = general_line_eq(x1, y1, x2, y2);
    double a = line_eq[0], b = line_eq[1], c = line_eq[2];
    if (a * x3 + b * y3 + c == 0)
        return true;    // They are the same line (المستقيمان منطبقان)
    return false;       // They are parllel
}
