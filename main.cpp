#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <algorithm>
#include <iostream>
#include <set>
#include <limits>
#include <iostream>
#include <fstream>
#include <string>
#include <deque>
#include <random>

using namespace std; 

// The BLF implementation is based in:
//------------------------------------------------------------------------------
// C++ BLF algorithm.
// 
// Copyright (c) 2012 Charles Dubout <charles.dubout@idiap.ch>
// 
// This file is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 3 as
// published by the Free Software Foundation.
// 
// It is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//------------------------------------------------------------------------------


// Rectangle object.
class Rectangle
{
private:
    // common variables. 
    int id; // rectangle id.
    bool rotated;   // is the rectangle rotated?

public:
    int width;  // rectangle width.
    int height; // rectangle height.
    
    //position variables.
    int x; // X coordinate of top-left corner.
    int y; // Y coordinate of top-left corner.

    //constructor @override.
    Rectangle(int given_id, int given_width, int given_height, int x_location, int y_location)
    {
        this->id = given_id;
        this->width = given_width;
        this->height = given_height;
        this->rotated = false;
        this->x = x_location;
        this->y = y_location;
    }

    //constructor @override.
    Rectangle(int given_id, int given_width, int given_height)
    {
        this->id = given_id;
        this->width = given_width;
        this->height = given_height;
        this->rotated = false;
    }

    int right() const //return right position of the rectangle.
    {
        return (x + width - 1);
    }

    int bottom() const    //return bottom position of the rectangle.
    {
        return(y + height -1);
    }

    int getID() //return id of the rectangle.
    {
        return id;
    }

    bool getRotation()  // return rotation of the rectangle.
    {
        return rotated;
    }

    void rotate()   // rotate the rectangle.
    {
        int new_width = height;
        int new_height = width;
        
        this->width = new_width;
        this->height = new_height;

        if(rotated == true)
        {
            this->rotated = false; 
        } 
        else
        {
            this->rotated = true;
        }
    }
};

struct PositionComparator
{
    bool operator()( const Rectangle a, const Rectangle b) const
    {
        return (a.y < b.y) ||
                         ((a.y == b.y) && 
                                    ((a.x < b.x) || 
                                                ((a.x == b.x) && 
                                                            ((a.height > b.height) || 
                                                                            ((a.height == b.height) && 
                                                                                            (a.width > b.width))))));
    }

};

//use BLF to locate the objects
int BLF (int given_surface_width, vector<Rectangle> &objects)
{
    int maximun_surface_height =  numeric_limits<int>::max();
    int calidad_maxima = 0;

    // Set of free gaps (Rects) in the image
    set<Rectangle, PositionComparator> gaps;

    // At the beginning, the whole image is free
	gaps.insert(Rectangle(-1, given_surface_width, maximun_surface_height, 0, 0));

    // Insert each Rect in the first gap big enough
	for (int i = 0; i<int(objects.size()); ++i) { // cast to int
		// Find the first gap big enough
		set<Rectangle, PositionComparator>::iterator g;
		
		for (g = gaps.begin(); g != gaps.end(); ++g)
			if ((g->width >= objects[i].width) && (g->height >= objects[i].height))
            {
                break;
            }
		
		// If a gap big enough was found
		if (g != gaps.end()) {
			// Insert the Rect in the gap
			objects[i].x = g->x;
			objects[i].y = g->y;

            if(g->y + objects[i].height >= calidad_maxima) calidad_maxima = g->y + objects[i].height;
			
			// Remove all the intersecting gaps, and add newly created gaps
			for (g = gaps.begin(); g != gaps.end();) {
				if (!((objects[i].right() < g->x) || (objects[i].bottom() < g->y) ||
					  (objects[i].x > g->right()) || (objects[i].y > g->bottom()))) {
					// Add a gap to the left of the new rectangle if possible
					if (g->x < objects[i].x)
						gaps.insert(Rectangle(-1, objects[i].x - g->x, g->height, g->x, g->y));
					
					// Add a gap on top of the new rectangle if possible
					if (g->y < objects[i].y)
						gaps.insert(Rectangle(-1, g->width, objects[i].y - g->y, g->x, g->y));
					
					// Add a gap to the right of the new rectangle if possible
					if (g->right() > objects[i].right())
						gaps.insert(Rectangle(-1, g->right() - objects[i].right(), g->height,
                        objects[i].right() + 1, g->y));
					
					// Add a gap below the new rectangle if possible
					if (g->bottom() > objects[i].bottom())
						gaps.insert(Rectangle(-1, g->width, g->bottom() - objects[i].bottom(),
                        g->x, objects[i].bottom() + 1));
					
					gaps.erase(g++); // Remove the intersecting gap
				}
				else {
					++g;
				}
			}
		}
	}
    return calidad_maxima;
}

int getInstance(string filename, vector<Rectangle> &objects , int &items)
{
    int id, width, height, aux;  //rectangle parameters and aux.
    int object_number, surface_width;   //number of objects, surface maximun width.
   
    //read the file.
    ifstream file(filename);
   
    //check if we can read the file.
    if(!file.good())
    {
        cout << "Can't read the file" << endl;
        return -1;
    }

    else
    {
        file >> aux;
        object_number = aux; // get the object_number from the file.
        items = object_number;   //set the number of objects/items.

        file >> aux;
        surface_width = aux; // get the object_number from the file.

        while(file >> id >> width >> height)
        {   
            Rectangle rectangle = Rectangle(id, width, height);
            objects.push_back(rectangle);
        }
    }
    file.close();
    return surface_width;
}

int unusedSpace(int &used_height, int &surface_width, vector<Rectangle> &solution)
{
    int used_space;
    int unused_space;
    int area = 0;

    used_space = used_height * surface_width;

    //calculate the used area in the surface.
    for (int iterator = 0 ; iterator < int(solution.size()); iterator ++)
    {
        area += (solution[iterator].width * solution[iterator].height);
    }

    unused_space = used_space - area;
    return unused_space;
}

int tabuSearch(int &surface_width, int &object_number, vector<Rectangle> initial_solution, vector<Rectangle> &best_solution)
{
    //tabu search init.
    bool terminate = false; // flag for the stop criterion.
    int criterion = 0;    //stop criterion , 10 iterations without improvement.

    float rotation = 0.4; //rotation probability as cake, if random <= 2 we rotate. //WE NEED TO SET THIS PARAMETER
    srand(time(NULL)); //seed for the random rotation.

    int tabu_length = 1*object_number; //tabu list lenght. //WE NEED TO SET THIS PARAMETER
    deque<int> tabu = deque<int>(); // empty tabu list.

    int candidate_value = BLF(surface_width, initial_solution); //initial solution as candidate solution.
    vector<Rectangle> candidate_object = initial_solution; //initial solution as candidate solution.

    int sbest_value = candidate_value; //candidate solution as best solution.
    vector<Rectangle> sbest_object = candidate_object; //candidate solution as best solution.

    //tabu serach start & generating the non-tabu neighborhood.
    while(!terminate)
    {   
        int best_neighbor_value = numeric_limits<int>::max();   //sv.
        vector<Rectangle> best_neighbor_object; //sv.
        int best_iterator;

        for(int iterator = 1; iterator < int(candidate_object.size()); iterator++)
        {   
            //randon rotation.
            if((static_cast <float> (rand()) / static_cast <float> (RAND_MAX)) <= rotation )    //rotate
            {
                candidate_object[int(static_cast <float> (rand()) / static_cast <float> (RAND_MAX/(object_number-1)))].rotate();
            }

            //check if movement is tabu.
            deque<int>::iterator tabu_find = find(tabu.begin(), tabu.end(), iterator);

            if(tabu_find != tabu.end()) //tabu found.
            {
                //do nothing, iterate again.
            }

            else    //not tabu.
            {
                int evaluation;

                vector<Rectangle> aux = candidate_object; //aux solution to swap.
                swap(aux[0],aux[iterator]);

                evaluation = BLF(surface_width, aux);

                //select the non-tabu best neighbor.
                if(evaluation < best_neighbor_value)
                {   
                    best_neighbor_value = evaluation;
                    best_neighbor_object = aux;
                    best_iterator = iterator;
                }
            }
        }
        candidate_value = best_neighbor_value;  //best neighbor is candidate.
        candidate_object = best_neighbor_object;    //best neighbor is candidate.

        //update tabu list.
        if(int(tabu.size()) < tabu_length)
        {
            tabu.push_front(best_iterator);
        }

        else
        {
            tabu.pop_back();
            tabu.push_front(best_iterator);
        }

        //check if candidate solution is better than the best solution.

        int candidate_unused = unusedSpace(candidate_value, surface_width, candidate_object);
        int sbest_unused = unusedSpace(sbest_value, surface_width, sbest_object);
        
        if((candidate_value < sbest_value && candidate_unused <= sbest_unused) || (candidate_value <= sbest_value && candidate_unused < sbest_unused))
        {
            sbest_value = candidate_value;
            sbest_object = candidate_object;
            
            criterion = 0;
        }

        else
        {
            criterion += 1;
        }

        //check stop criterion.
        if(criterion > 10)
        {
            terminate = true;
        }
    }
    best_solution = sbest_object;
    return sbest_value;
}

int outPut(string filename, int &object_number, int &used_height, int &surface_width, vector<Rectangle> &solution)
{
    string output_filename;

    string delimiter = ".";
    output_filename = filename.substr(0,filename.find(delimiter));
    output_filename.append(".output");

    ofstream file(output_filename);

    //check if we can write the file.
    if(!file.good())
    {
        cout << "Can't write the file" << endl;
        return -1;
    }

    file << used_height << endl;
    file << unusedSpace(used_height, surface_width, solution) << endl;

    for(int iterator = 0; iterator < int(solution.size()); iterator++)
    {
        int rotation;

        if(solution[iterator].getRotation()) rotation = 1;
        else rotation = 0;

        file << solution[iterator].x << " " << solution[iterator].y << " " << rotation << endl;
    }

    file.close();
    return 0;
}

int main(int args, char **argv){
    
    int surface_width;  //surface width.
    int object_number;  //number of objects
    int output_status;  //write file status.

    string instance_name = argv[1];   // instance filename to open.
    string instance = "2sp/" + instance_name; //append the instance filename to the directory string.
    vector<Rectangle> rectangles =  vector<Rectangle>();   //vector of objects.

    string output = "output/" + instance_name;  //output filename.
    
    surface_width = getInstance(instance, rectangles, object_number); //read the file and return the surface width

    if(surface_width == -1)
    {
        return 1; //cant read the file.
    }

    else
    {
        int solution_height; //solution height.
        vector<Rectangle> solution =  vector<Rectangle>();   //vector of objects for the solution.
        solution_height = tabuSearch(surface_width, object_number, rectangles, solution);
        output_status = outPut(output, object_number, solution_height, surface_width, solution);

        if(output_status == -1) return 1; //cant write the file.
    }
    return 0;
}