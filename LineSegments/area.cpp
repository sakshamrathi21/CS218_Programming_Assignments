#include <cmath>
#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;

float epsilon = 1e-3;

class Point {
	public:
	float x;
	float y;

	public:
	Point()
		: x(0), y(0) {}  // Default constructor initializing coordinates to (0,0)
	Point(float xVal, float yVal)
		: x(xVal), y(yVal) {}  // Parameterized constructor

	float X() const { return x; }  // Accessor for x coordinate
	float Y() const { return y; }  // Accessor for y coordinate
    void Print() {
        cout << x << " " << y << endl;
    }

};

 

bool compare_points(const pair<Point, Point> &posterA, const pair<Point, Point> &posterB) {
    if (abs(posterB.first.X()-posterA.first.X()) > epsilon) return posterA.first.X() < posterB.first.X();
    else return posterA.second.X() < posterB.second.X();
}


float length_x_axis(const vector <pair<Point, Point> > &posters) {
    int n = posters.size();
    if (n <= 0) return 0;
    float length = posters[0].second.X() - posters[0].first.X();
    float end_point = posters[0].second.X();
	
    for (int i = 1 ; i < n ; i ++) {
        if (posters[i].first.X() >= end_point) {
            length += posters[i].second.X() - posters[i].first.X();
            end_point = posters[i].second.X();
        }
        else if (posters[i].second.X() >= end_point) {
            length += posters[i].second.X() - end_point;
            end_point = posters[i].second.X();
        }
    }
	return length;
}

bool onSegment(Point p, Point q, Point r) 
{ 
    if (q.x < max(p.x, r.x) && q.x > min(p.x, r.x) && 
        q.y < max(p.y, r.y) && q.y > min(p.y, r.y)) 
       return true; 
  
    return false; 
} 

int orientation(Point p, Point q, Point r) 
{ 
    // See https://www.geeksforgeeks.org/orientation-3-ordered-points/ 
    // for details of below formula. 
    int val = (q.y - p.y) * (r.x - q.x) - 
              (q.x - p.x) * (r.y - q.y); 
  
    if (val == 0) return 0;  // collinear 
  
    return (val > 0)? 1: 2; // clock or counterclock wise 
} 

bool doLineSegmentsIntersect(Point p1, Point q1, Point p2, Point q2) {
    int o1 = orientation(p1, q1, p2); 
    int o2 = orientation(p1, q1, q2); 
    int o3 = orientation(p2, q2, p1); 
    int o4 = orientation(p2, q2, q1); 
  
    // General case 
    if (o1 != o2 && o3 != o4) 
        return true; 
  
    // Special Cases 
    // p1, q1 and p2 are collinear and p2 lies on segment p1q1 
    if (o1 == 0 && onSegment(p1, p2, q1)) return true; 
  
    // p1, q1 and q2 are collinear and q2 lies on segment p1q1 
    if (o2 == 0 && onSegment(p1, q2, q1)) return true; 
  
    // p2, q2 and p1 are collinear and p1 lies on segment p2q2 
    if (o3 == 0 && onSegment(p2, p1, q2)) return true; 
  
     // p2, q2 and q1 are collinear and q1 lies on segment p2q2 
    if (o4 == 0 && onSegment(p2, q1, q2)) return true; 
  
    return false; // Doesn't fall in any of the above cases 
}

Point lineSegmentIntersectionPoint(pair<Point, Point> posterA, pair<Point, Point> posterB) {
    if (!doLineSegmentsIntersect(posterA.first, posterA.second, posterB.first, posterB.second)) return Point(0, 0);
    float a1 = posterA.second.Y() - posterA.first.Y();
    float b1 = posterA.first.X() - posterA.second.X();
    float c1 = a1*(posterA.first.X()) + b1*posterA.first.Y();
    float a2 = posterB.second.Y() - posterB.first.Y();
    float b2 = posterB.first.X() - posterB.second.X();
    float c2 = a2*(posterB.first.X()) + b2*(posterB.first.Y());
    float determinant = a1*b2 - a2*b1;
    float x = (b2*c1 - b1*c2)/determinant;
    float y = (a1*c2 - a2*c1)/determinant;
    return Point(x, y);
}


float trapezium_area_calculator(const pair<Point, Point> &posterA) {
    if (posterA.first.X() < posterA.second.X()) {
        float result = (posterA.second.X() - posterA.first.X())*(posterA.second.Y() + posterA.first.Y());
        result /= 2;
        return result;
    }
    else return 0;
}

bool valid_trapezium(const pair<Point, Point> &posterA) {
    if (isnan(posterA.first.X()) || isnan(posterA.second.X()) || isnan(posterA.first.Y()) || isnan(posterA.second.Y())) return false;
    if (posterA.first.X() > posterA.second.X()) return false;
    if (abs(posterA.first.X() - posterA.second.X()) < epsilon) return false;
    else return true;
}

int extra_left = 0;

bool compare_points_id(const pair<Point, int> &point_id_1, const pair<Point, int> &point_id_2) {
    return point_id_1.first.x < point_id_2.first.x;
}

float corresponding_y(float x_val, const pair<Point, Point> &posterA) {
    if (x_val < posterA.first.x || x_val > posterA.second.x) return 0;
    return ((posterA.second.y - posterA.first.y)*(x_val - posterA.first.x)/(posterA.second.x - posterA.first.x) + posterA.first.y);
}

int which_is_higher(float x_val, const pair<Point, Point> &posterA, const pair<Point, Point> &posterB) {
    if (corresponding_y(x_val, posterA) > corresponding_y(x_val, posterB)) return 1;
    else return 2;
}

vector <pair<Point, Point> > two_trapezium_merger(const pair<Point, Point> &posterA, const pair<Point, Point> &posterB) {
    
    if (posterB.first.x >= posterA.second.x) {
        vector <pair<Point, Point> > modified_posters1;
        modified_posters1.push_back(posterA); modified_posters1.push_back(posterB);
        extra_left = 2;
        return modified_posters1;
    }
    vector <pair<Point, Point> > modified_posters(4);
    if (!doLineSegmentsIntersect(posterA.first, posterA.second, posterB.first, posterB.second)) {
        // pair<Point, int> point_id_1, point_id_2, point_id_3, point_id_4;
        vector<pair<Point, int> > all_points_ids(4);
        if (posterA.first.x <= posterB.second.x && posterA.first.x >= posterB.first.x) {
            float corres_y = corresponding_y(posterA.first.x, posterB);
            if (corres_y > posterA.first.y) {
                all_points_ids[0].first = Point(posterA.first.x, corres_y);
                all_points_ids[0].second = 2;
            }
            else {
                all_points_ids[0].first = posterA.first;
                all_points_ids[0].second = 1;
            }
        }
        else {
            all_points_ids[0].first = posterA.first;
            all_points_ids[0].second = 1;
        }
        if (posterA.second.x <= posterB.second.x && posterA.second.x >= posterB.first.x) {
            float corres_y = corresponding_y(posterA.second.x, posterB);
            if (corres_y > posterA.second.y) {
                all_points_ids[1].first = Point(posterA.second.x, corres_y);
                all_points_ids[1].second = 2;
            }
            else {
                all_points_ids[1].first = posterA.second;
                all_points_ids[1].second = 1;
            }
        }
        else {
            all_points_ids[1].first = posterA.second;
            all_points_ids[1].second = 1;
        }
        if (posterB.first.x <= posterA.second.x && posterB.first.x >= posterA.first.x) {
            float corres_y = corresponding_y(posterB.first.x, posterA);
            if (corres_y > posterB.first.y) {
                all_points_ids[2].first = Point(posterB.first.x, corres_y);
                all_points_ids[2].second = 1;
            }
            else {
                all_points_ids[2].first = posterB.first;
                all_points_ids[2].second = 2;
            }
        }
        else {
            all_points_ids[2].first = posterB.first;
            all_points_ids[2].second = 2;
        }
        if (posterB.second.x <= posterA.second.x && posterB.second.x >= posterA.first.x) {
            float corres_y = corresponding_y(posterB.second.x, posterA);
            if (corres_y > posterB.second.y) {
                all_points_ids[3].first = Point(posterB.second.x, corres_y);
                all_points_ids[3].second = 1;
            }
            else {
                all_points_ids[3].first = posterB.second;
                all_points_ids[3].second = 2;
            }
        }
        else {
            all_points_ids[3].first = posterB.second;
            all_points_ids[3].second = 2;
        }
        // vector<pair<Point, int> > all_points_ids;
        // all_points_ids.push_back(point_id_1); all_points_ids.push_back(point_id_2); all_points_ids.push_back(point_id_3); all_points_ids.push_back(point_id_4);
        sort(all_points_ids.begin(), all_points_ids.end(), compare_points_id);
        int which_higher;
        for (int l = 0 ; l < all_points_ids.size()-1 ; l ++ ) {
            // cout << " sita " << endl;if (all_points_ids[l].first.x <= all_points_ids[l+1].first.x) {
            if (all_points_ids[l].second == all_points_ids[l+1].second) {
                modified_posters[l] = (make_pair(all_points_ids[l].first, all_points_ids[l+1].first));
                which_higher = all_points_ids[l].second;
            }
            else {
                which_higher = which_is_higher((all_points_ids[l].first.x+all_points_ids[l+1].first.x)/2, posterA, posterB);
                if (which_higher == 1) {
                    modified_posters[l] = (make_pair(Point(all_points_ids[l].first.x, corresponding_y(all_points_ids[l].first.x, posterA)), Point(all_points_ids[l+1].first.x, corresponding_y(all_points_ids[l+1].first.x, posterA))));
                }
                else {
                    modified_posters[l] = (make_pair(Point(all_points_ids[l].first.x, corresponding_y(all_points_ids[l].first.x, posterB)), Point(all_points_ids[l+1].first.x, corresponding_y(all_points_ids[l+1].first.x, posterB))));
                }
            }
        }
        modified_posters.pop_back();
        extra_left = which_higher;
    }
    else {
        // pair<Point, int> point_id_1, point_id_2, point_id_3, point_id_4, point_id_5;
        vector<pair<Point, int> > all_points_ids(5);
        if (posterA.first.x <= posterB.second.x && posterA.first.x >= posterB.first.x) {
            float corres_y = corresponding_y(posterA.first.x, posterB);
            if (corres_y > posterA.first.y) {
                all_points_ids[0].first = Point(posterA.first.x, corres_y);
                all_points_ids[0].second = 2;
            }
            else {
                all_points_ids[0].first = posterA.first;
                all_points_ids[0].second = 1;
            }
        }
        else {
            all_points_ids[0].first = posterA.first;
            all_points_ids[0].second = 1;
        }
        if (posterA.second.x <= posterB.second.x && posterA.second.x >= posterB.first.x) {
            float corres_y = corresponding_y(posterA.second.x, posterB);
            if (corres_y > posterA.second.y) {
                all_points_ids[1].first = Point(posterA.second.x, corres_y);
                all_points_ids[1].second = 2;
            }
            else {
                all_points_ids[1].first = posterA.second;
                all_points_ids[1].second = 1;
            }
        }
        else {
            all_points_ids[1].first = posterA.second;
            all_points_ids[1].second = 1;
        }
        if (posterB.first.x <= posterA.second.x && posterB.first.x >= posterA.first.x) {
            float corres_y = corresponding_y(posterB.first.x, posterA);
            if (corres_y > posterB.first.y) {
                all_points_ids[2].first = Point(posterB.first.x, corres_y);
                all_points_ids[2].second = 1;
            }
            else {
                all_points_ids[2].first = posterB.first;
                all_points_ids[2].second = 2;
            }
        }
        else {
            all_points_ids[2].first = posterB.first;
            all_points_ids[2].second = 2;
        }
        if (posterB.second.x <= posterA.second.x && posterB.second.x >= posterA.first.x) {
            float corres_y = corresponding_y(posterB.second.x, posterA);
            if (corres_y > posterB.second.y) {
                all_points_ids[3].first = Point(posterB.second.x, corres_y);
                all_points_ids[3].second = 1;
            }
            else {
                all_points_ids[3].first = posterB.second;
                all_points_ids[3].second = 2;
            }
        }
        else {
            all_points_ids[3].first = posterB.second;
            all_points_ids[3].second = 2;
        }
        all_points_ids[4].first = lineSegmentIntersectionPoint(posterA, posterB);
        all_points_ids[4].second = 1;
        // cout << " ram " << endl;
        // cout << point_id_1.first.x << " " << point_id_1.first.y << " " << point_id_1.second << endl;
        // cout << point_id_2.first.x << " " << point_id_2.first.y << " " << point_id_2.second << endl;
        // cout << point_id_3.first.x << " " << point_id_3.first.y << " " << point_id_3.second << endl;
        // cout << point_id_4.first.x << " " << point_id_4.first.y << " " << point_id_4.second << endl;
        // cout << point_id_5.first.x << " " << point_id_5.first.y << " " << point_id_5.second << endl;
        // cout << " ram " << endl;
        
        // all_points_ids.push_back(point_id_1); all_points_ids.push_back(point_id_2); all_points_ids.push_back(point_id_3); all_points_ids.push_back(point_id_4), all_points_ids.push_back(point_id_5);
        sort(all_points_ids.begin(), all_points_ids.end(), compare_points_id);
        int which_higher;
        for (int l = 0 ; l < all_points_ids.size()-1 ; l ++ ) {
            // cout << " sita " << endl;
            
            if (all_points_ids[l].second == all_points_ids[l+1].second) {
                modified_posters[l] = (make_pair(all_points_ids[l].first, all_points_ids[l+1].first));
                which_higher = all_points_ids[l].second;
            }
            else {
                which_higher = which_is_higher((all_points_ids[l].first.x+all_points_ids[l+1].first.x)/2, posterA, posterB);
                if (which_higher == 1) {
                    modified_posters[l] = (make_pair(Point(all_points_ids[l].first.x, corresponding_y(all_points_ids[l].first.x, posterA)), Point(all_points_ids[l+1].first.x, corresponding_y(all_points_ids[l+1].first.x, posterA))));
                }
                else {
                    modified_posters[l] = (make_pair(Point(all_points_ids[l].first.x, corresponding_y(all_points_ids[l].first.x, posterB)), Point(all_points_ids[l+1].first.x, corresponding_y(all_points_ids[l+1].first.x, posterB))));
                }
            }
        }
        extra_left = which_higher;
    }
    return modified_posters;
}

vector <pair<Point, Point> > lets_merge_trapeziums(vector <pair<Point, Point> > postersA, vector <pair<Point, Point> > postersB) {
    if (postersA.size() == 0) return postersB;
    if (postersB.size() == 0) return postersA;
    int left_index = 0;
    int right_index = 0;
    int size1 = postersA.size();
    int size2 = postersB.size();
    // bool flip = false;
    vector <pair<Point, Point> > merged_trapeziums;
    while (left_index < size1 && right_index < size2) {
        // cout << " ram " << left_index << size1 << right_index << size2 << extra_left<< endl;;
        extra_left = 0;
        pair<Point, Point> posterA = postersA[left_index];
        pair<Point, Point> posterB = postersB[right_index];
        vector <pair<Point, Point> > merged_two = two_trapezium_merger(posterA, posterB);
        // if (posterA.first.X() <= posterB.first.X()) {
        //     merged_two = 
        //     // flip = false;
        // }
        
        if (extra_left == 1) {
            pair<Point, Point> extra_trapezium = merged_two[merged_two.size()-1];
            // merged_two.pop_back();
            for (int t = 0 ; t < merged_two.size()-1 ; t++) {
                if (valid_trapezium(merged_two[t])) merged_trapeziums.push_back(merged_two[t]);
            }
            right_index++;
            postersA[left_index] = extra_trapezium;
        }
        else {
            pair<Point, Point> extra_trapezium = merged_two[merged_two.size()-1];
            // merged_two.pop_back();
            for (int t = 0 ; t < merged_two.size() -1; t++) {
                if (valid_trapezium(merged_two[t])) merged_trapeziums.push_back(merged_two[t]);
            }
            left_index++;
            postersB[right_index] = extra_trapezium;
        }
    }
    while (left_index < size1) {
        if (valid_trapezium(postersA[left_index])) merged_trapeziums.push_back(postersA[left_index]);
        left_index++;
    }
    while (right_index < size2) {
        if (valid_trapezium(postersB[right_index])) merged_trapeziums.push_back(postersB[right_index]);
        right_index++;
    }
    return merged_trapeziums;
}

vector <pair<Point, Point> > lets_divide_trapeziums(vector <pair<Point, Point> > &posters, int left, int right) {
    if (right-left==1) {
        vector <pair<Point, Point> > posters_return;
        posters_return.push_back(posters[left]);
        return posters_return;   
    }   
    // vector <pair<Point, Point> > part1, part2;
    // copy(posters.begin(), posters.begin() + posters.size()/2, back_inserter(part1));
    // copy(posters.begin() + posters.size()/2, posters.end(), back_inserter(part2));
    return lets_merge_trapeziums(lets_divide_trapeziums(posters, left, (left+right)/2), lets_divide_trapeziums(posters, (left+right)/2, right ));
}



int main(){
	   /* Enter your code here. Read input from STDIN. Print output to STDOUT */   
    ios::sync_with_stdio(0);
    cin.tie(0);
    // cout << " start " << endl;
	int n;
	cin >> n;
	float a,b,c,d;
    vector <pair<Point, Point> > posters(n);
	for(int i=0; i<n; i++){
		cin >> a;
		cin >> b;
		cin >> c;
		cin >> d;
		posters[i] = make_pair(Point(a,b), Point(c,d));	
	}
	sort(posters.begin(), posters.end(), compare_points);
	float lengthCovered = length_x_axis(posters); 
    cout << static_cast<int>(lengthCovered)<< '\n';
    vector <pair<Point, Point> > all_merged_trapeziums = lets_divide_trapeziums(posters, 0, n);
    float area = 0;
    float area_length = 0;
    vector<bool> participating(all_merged_trapeziums.size(), true);
    int last_participating = 0;
    for (int i = 0 ; i < all_merged_trapeziums.size() ; i ++ ) {
        if (i > 0 && all_merged_trapeziums[i].first.X() < all_merged_trapeziums[last_participating].second.X()) {
            participating[i] = false;
        }
        else {
            last_participating = i;
        }
    }
    for (int i = 0 ; i < all_merged_trapeziums.size() ; i ++ ) {
        if (participating[i]) {
            area_length += all_merged_trapeziums[i].second.X() - all_merged_trapeziums[i].first.X();
            area += trapezium_area_calculator(all_merged_trapeziums[i]);
        }
    }
	
	cout << "Length from area algorithm: " << (area_length) << '\n';
	cout << static_cast<int>(area)<< '\n';

	return 0;
}

