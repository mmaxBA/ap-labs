package main

import "golang.org/x/tour/pic"

func Pic(dx, dy int) [][]uint8 {
	var a [][]uint8
	a = make([][]uint8, dy)
	for i := 0; i < len(a); i++ {
		a[i] = make([]uint8, dx)
		for j := 0; j < dx; j++{
			a[i][j] =uint8(i)*uint8(j)/2
		}
	}
	return a
}

func main() {
	pic.Show(Pic)
}

