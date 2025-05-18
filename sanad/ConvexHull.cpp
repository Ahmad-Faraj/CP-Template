/* Convex Hull */
bool inBetween(vi p1, vi p2, vi p3) {
    int x1 = p1.front(), y1 = p1.back();
    int x2 = p2.front(), y2 = p2.back();
    int x3 = p3.front(), y3 = p3.back();
    bool b1 = (x1 <= x2 && x2 <= x3) || (x3 <= x2 && x2 <= x1);
    bool b2 = (y1 <= y2 && y2 <= y3) || (y3 <= y2 && y2 <= y1);
}

int orientation(vector<int> p1, vector<int> p2, vector<int> p3){
    int x1 = p1.front(), y1 = p1.back();
    int x2 = p2.front(), y2 = p2.back();
    int x3 = p3.front(), y3 = p3.back();
    int diff = (y3 - y2) * (x2 - x1) - (y2 - y1) * (x3 - x2);

    if (diff > 0) return 1;     // CounterClockWise
    if (diff < 0) return -1;    // ClockWise
    /* if (diff == 0) */  return 0;   // collinear
}

vector < vector <int> > Jarvis(vector <vector <int> > p) {
    int extream_point_idx = 0;
    int n = p.size();
    for (int i = 0; i < n; i++)
        if (p[i].front() < p[extream_point_idx].front())
            extream_point_idx = i;
    
    vector < vector <int> > Hull_Ans;
    int on_hull = extream_point_idx;

    do
    {
        Hull_Ans.push_back(p[on_hull]);
        int next_p = 0;
        for (int i = 0; i < n; i++){
            int o = orientation(p[on_hull], p[next_p], p[i]);
            if (next_p == on_hull || o == 1)
                next_p = i;
        }
        for (int i = 0; i < n; i++){
            bool b1 = i != on_hull && i != next_p;
            bool b2 = orientation(p[on_hull], p[i], p[next_p]) == 0;
            bool b3 = inBetween(p[on_hull], p[i], p[next_p]);
            if (b1 && b2 && b3)
                Hull_Ans.push_back(p[i]);
        }
        on_hull = next_p;
    } while (p[on_hull] != Hull_Ans[0]);
    

    return Hull_Ans;
}
