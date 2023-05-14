#ifndef SHAPE_H_
#define SHAPE_H_

#include<iostream>
#include<cmath>
#include<string>
#include<algorithm>
#include<list>
#include<fstream>
#include <iomanip>    

using namespace std;
/*
INTRO: For this Shape class, I set the private name attribute in the constructor, and returned the private name attribute using the getName function.
For the Rectangle/Circle/Sphere/RectPrism classes, I first set the private attributes using in the constructors (width, length; radius; radius; width,
length, height respectively), returning these private attributes in the respective get functions. I implemented the getArea, getVolume, +, and - functions
as detailed on the wiki, making sure to convert the type of the width and length to the passed type in the rectangle's operator functions, and making sure
to convert the dimension to 0 if any classes subtract function returned a value less than 0. For CreateShapes, I first used ifstream to get the number of
shapes from the passed document, then iterated through every shape, first reading the name attribute into a variable to determine the type of the shape,
then reading the dimensions into variables based on the type of the shape. After this, I allocated memory for a new Shape of the determined type using the
read dimensions, then added the pointer to the list to be returned using push_back. For MaxArea I used an iterator to go through the passed list of shape
pointers, using a variable to record the area of each shape (gotten from getArea), and comparing the area to a variable for the max area. If the new area
was greater than the max area, I updated the max area variable. For MaxVolume I did the same thing, but calling getVolume for each shape instead.
 */


//Base class
//Please implement Shape's member functions
//constructor, getName()
class Shape{
public:
	//Base class' constructor should be called in derived classes'
	//constructor to initizlize Shape's private variable 
  	Shape(string name) {
	  name_ = name; //shape constructor only needs to initialize name of Shape variable
	}
	
  	string getName() {
	  return name_; //return name_ since it's a private attribute
	}
	
  	virtual double getArea() const = 0;
  	virtual double getVolume() const = 0;
	
private:
  string name_;


};

//Rectangle
//Please implement the member functions of Rectangle:
//constructor, getArea(), getVolume(), operator+, operator-

template <class T>
class Rectangle : public Shape{
public:
  	Rectangle<T>(T width = 0, T length = 0):Shape("Rectangle") {
	  width_ = width; //initialize private variable width_
	  length_ = length; //initialize private variable length_
	}
	
  	double getArea() const {
	  return double(width_ * length_); //area of rectangle = w*l
	}
	
  	double getVolume() const {
	  return double(0); //rectangle has no volume
	}
	
	Rectangle<T> operator + (const Rectangle<T>& rec) {
	  double new_width, new_length; //initialize variables for width and height of new rectangle after addition
	  new_length = length_ + (T)rec.getLength(); //add lengths of rectangles
	  new_width = width_ + (T)rec.getWidth(); //add widths of rectangles
	  Rectangle<T> return_rec(new_width, new_length); //create new rectangle with new dimensions using constructor
	  return return_rec; //return new rectangle
	  
	}
	
	Rectangle<T> operator - (const Rectangle<T>& rec) {
	  double new_width, new_length; //initialize variables for width and height of new rectangle after addition
	  new_length = max(length_ - (T)rec.getLength(), (T)0); //subtract lengths of rectangles, returning 0 if result is negative
	  new_width = max(width_ - (T)rec.getWidth(), (T)0); //subtract widths of rectangles, returning 0 if result is negative
	  Rectangle<T> return_rec(new_width, new_length); //create new rectangle with new dimensions using constructor
	  return return_rec; //return new rectangle
	} 
	
	T getWidth() const { 
	  return width_;
	}
	
	T getLength() const { 
	  return length_;
	}
private:
	T width_;
        T length_;

};

//Circle
//Please implement the member functions of Circle:
//constructor, getArea(), getVolume(), operator+, operator-
//@@Insert your code here

class Circle : public Shape{
public:
  	Circle(double radius):Shape("Circle") {
	  radius_ = radius; //initialize private variable radius_
	}
	
  	double getArea() const{
	  return (radius_ * radius_) * M_PI; //area of circle = pi*r^2
	}
	
 	double getVolume() const{
	  return double(0); //circle has no volume
	}
	
  	Circle operator + (const Circle& cir){
	  double new_rad;
	  new_rad = radius_ + cir.getRadius(); //add radii of circles
	  Circle new_circle(new_rad); //create new circle with radius new_rad
	  return new_circle;
	}
	
	Circle operator - (const Circle& cir){
	  double new_rad;
	  new_rad = max(radius_ - cir.getRadius(), 0.0); //subtract radii of circles, returning 0 if result is negative
	  Circle new_circle(new_rad); //create new circle with radius new_rad
	  return new_circle;
	} 

	double getRadius() const { 
	  return radius_;
	}
	
private:
	double radius_;

};



//Sphere
//Please implement the member functions of Sphere:
//constructor, getArea(), getVolume(), operator+, operator-
//@@Insert your code here

class Sphere : public Shape{
public:
  	Sphere(double radius):Shape("Sphere") {
	  radius_ = radius;
	}

  	double getVolume() const {
	  return (4.0/3.0) * (radius_ * radius_ * radius_) * M_PI; //volume of sphere = (4/3)*pi*r^3
	}	
	
  	double getArea() const {
	  return 4.0 * M_PI * (radius_ * radius_); //area of sphere = 4*pi*r^2
	}

	Sphere operator + (const Sphere& sph) {
	  double new_rad;
	  new_rad = radius_ + sph.getRadius(); //add radii of spheres
	  Sphere new_sphere(new_rad); //create new sphere with radius new_rad
	  return new_sphere;	  
	}

	Sphere operator - (const Sphere& sph) {
	  double new_rad;
	  new_rad = max(radius_ - sph.getRadius(), 0.0); //subtract radii of spheres, returning 0 if result is negative
	  Sphere new_sphere(new_rad); //create new sphere with radius new_rad
	  return new_sphere;
	} 
	
	double getRadius() const {
	  return radius_;
	}

private:
	double radius_;

};

//Rectprism
//Please implement the member functions of RectPrism:
//constructor, getArea(), getVolume(), operator+, operator-
//@@Insert your code here
class RectPrism : public Shape{
public:
  	RectPrism(double width, double length, double height):Shape("RectPrism") {
	  length_ = length; //initialize private variable length_
	  width_ = width; //initialize private variable width_
	  height_ = height; //initialize private variable height_
	}
	
  	double getVolume() const {
	  return length_ * width_ * height_; //volume of rectangular prism = l*w*h
	}
  	
	double getArea() const {
	  return 2.0 * ((length_ * width_) + (length_ * height_) + (width_ * height_)); //area of rectangular prism is sum of areas of each face
	}
	
	RectPrism operator + (const RectPrism& rectp){
	  double new_length, new_width, new_height; //initialize variables for dimensions of new prism
	  new_length = length_ + rectp.getLength(); //add lengths of prisms
	  new_width = width_ + rectp.getWidth(); //add widths of prisms
	  new_height = height_ + rectp.getHeight(); //add heights of prisms
	  RectPrism new_prism(new_width, new_length, new_height); //create new prism with new dimensions
	  return new_prism;
	}
	
	RectPrism operator - (const RectPrism& rectp){
	  double new_length, new_width, new_height; //initialize variables for dimensions of new prism
	  new_length = max(length_ - rectp.getLength(), 0.0); //subtract lengths of prisms, returning 0 if result is less than 0
	  new_width = max(width_ - rectp.getWidth(), 0.0); //subtract widths of prisms, returning 0 if result is less than 0
	  new_height = max(height_ - rectp.getHeight(), 0.0); //subtract heights of prisms, returning 0 if result is less than 0
	  RectPrism new_prism(new_width, new_length, new_height); //create new prism with new dimensions
	  return new_prism;
	}	
	
	double getWidth() const { 
	  return width_;
	}
	
	double getLength() const { 
	  return length_;
	}
	
	double getHeight() const { 
	  return height_;
	}
  
private:
  	double length_;
  	double width_;
  	double height_;

};

// Read shapes from test.txt and initialize the objects
// Return a vector of pointers that points to the objects 
static list<Shape*> CreateShapes(char* file_name) {
  list<Shape*> return_list; //initialize list of shape pointers to be returned
  int object_num; //initialize variable for number of objects from test.txt
  string name; //initialize variable for names of objects
  ifstream ifs (file_name, std::ifstream::in); //open passed file using ifstream
  ifs >> object_num; //read number of objects from text.txt into object_num
  for (int i = 0; i < object_num; i++) { //iterate through all shapes
    ifs >> name; //read name of shape into name
    if (name == "Rectangle") {
      double width, length; //initialize variables for width and length of rectangle
      ifs >> width >> length; //read width and length into variables from file
      Shape *rect_ptr = new Rectangle<double>(width, length); //create pointer to new rectangle using width and length
      return_list.push_back(rect_ptr); //insert pointer to new rectangle in list
    }
    else if (name == "Circle") {
      double radius; //initialize variable for radius of circle
      ifs >> radius; //if we have circle, read radius into variable
      Shape *circle_ptr = new Circle(radius);  //create pointer to new circle with read radius dimension
      return_list.push_back(circle_ptr); //insert pointer to new circle in list
    }
    else if (name == "Sphere") {
      double radius; //initialize variable for radius of sphere
      ifs >> radius; //read radius into variable
      Shape *sphere_ptr = new Sphere(radius); //create pointer to new sphere using radius
      return_list.push_back(sphere_ptr); //insert pointer to new sphere in list
    }
    else if (name == "RectPrism") {
      double width, length, height; //initialize variables for width, length, and height of rectangle
      ifs >> width >> length >> height; //read dimensions into variables
      Shape *prism_ptr = new RectPrism(width, length, height); //create pointer to new prism using passed dimensions
      return_list.push_back(prism_ptr); //insert pointer to new prism in list
    }
  }
  return return_list;
}

// call getArea() of each object 
// return the max area
static double MaxArea(list<Shape*> shapes){
	double max_area = 0;
	double temp_area;
	for (list<Shape*>:: iterator it = shapes.begin(); it != shapes.end(); it++){ //iterate through list of shapes
	  temp_area = 0; //variable for area of shape, to be compared to max_area
	  temp_area = (*it)->getArea(); //dereference it to get shape pointer it refers to, use getArea function from pointer
	  if (temp_area > max_area) {
	    max_area = temp_area; //if temp_area is greater than our current max area, we need to update the max area
	  }
	}

	return max_area;
}

// call getVolume() of each object 
// return the max volume
static double MaxVolume(list<Shape*> shapes){
	double max_volume = 0;
        double temp_volume; //variable for volume of shape, to be compared to max_volume
	for (list<Shape*>:: iterator it = shapes.begin(); it != shapes.end(); it++){ //iterate through list of shapes
	  temp_volume = (*it)->getVolume(); //dereference it to get shape pointer it refers to, use getVolume function from pointer
	  if (temp_volume > max_volume) {
	    max_volume = temp_volume; //if temp_volume is greater than our current max volume, we need to update the max volume
	  }
	}
	
	return max_volume;
}
#endif

