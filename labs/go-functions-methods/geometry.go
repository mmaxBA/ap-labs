package main

import (
	"fmt"
	"math"
	"math/rand"
	"os"
	"strconv"
	"time"
)

type Point struct{ x, y float64 }

// traditional function
func Distance(p, q Point) float64 {
	return math.Hypot(q.x-p.x, q.y-p.y)
}

// same thing, but as a method of the Point type
func (p Point) Distance(q Point) float64 {
	return math.Hypot(q.x-p.x, q.y-p.y)
}

//!-point

//!+path

// A Path is a journey connecting the points with straight lines.
type Path struct{
	points []Point
}

// Distance returns the distance traveled along the path.
func (path Path) Distance() float64 {
	sum := 0.0
	for i := range path.points {
		if i > 0 {
			sum += path.points[i-1].Distance(path.points[i])
		}
	}
	return sum
}

func generatePoints(n int) []Point {
	min := -100.0
	max := 100.0
	res := make([]Point, n)
	res[0] = Point{
		x: min + rand.Float64() * (max - min) ,
		y:  min + rand.Float64() * (max - min),
	}
	res[1] = Point{
		x: min + rand.Float64() * (max - min) ,
		y:  min + rand.Float64() * (max - min),
	}
	for i := 2; i < n; i++{
		res[i] = Point{
			x: min + rand.Float64() * (max - min) ,
			y:  min + rand.Float64() * (max - min),
		}
		for j:= 0; j< i-2; j++{
			for doIntersect(res[j], res[j+1], res[i-1], res[i]) {
				fmt.Print("Hola")
				res[i] = Point{
					x: min + rand.Float64() * (max - min) ,
					y:  min + rand.Float64() * (max - min),
				}
			}
		}
	}
	return res
}

func  onSegment(p,  q, r Point ) bool {
	if q.x <= math.Max(p.x, r.x) && q.x >= math.Min(p.x, r.x) &&  q.y <= math.Max(p.y, r.y) && q.y >= math.Min(p.y, r.y) {
		return true
	}
	return false
}

func orientation(p, q, r Point) int{
	val := (q.y - p.y) * (r.x - q.x) -  (q.x - p.x) * (r.y - q.y)
	if val == 0{
		return 0
	}
	if val > 0 {
		return 1
	}else{
		return 2
	}
}

func doIntersect(p1, q1, p2, q2 Point) bool{

	o1 := orientation(p1, q1, p2)
	o2 := orientation(p1, q1, q2)
	o3 := orientation(p2, q2, p1)
	o4 := orientation(p2, q2, q1)

	if o1 != o2 && o3 != o4 {
		return true
	}
	if o1 == 0 && onSegment(p1, p2, q1) {
		return true
	}
	if o2 == 0 && onSegment(p1, q2, q1) {
		return true
	}
	if o3 == 0 && onSegment(p2, p1, q2) {
		return true
	}
	if o4 == 0 && onSegment(p2, q1, q2) {
		return true
	}
	return false
}

func main(){
	sides, err := strconv.Atoi(os.Args[1])
	if err == nil {
		rand.Seed(time.Now().UnixNano())
		path :=Path{points:generatePoints(sides)}
		fmt.Println(path.points)
		for i := 0; i < sides-1; i++{
			fmt.Print(path.points[i].Distance(path.points[i+1]))
			fmt.Print("+")
		}
		fmt.Println(path.points[0].Distance(path.points[sides-1]))
		fmt.Println(path.Distance())


	}
}

//!-path
