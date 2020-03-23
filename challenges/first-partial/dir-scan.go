package main

import (
	"fmt"
	"os"
	"path/filepath"
)

type Result struct{
	path string
	dirs int
	symblink int
	devices int
	sockets int
	misc int
}



// scanDir stands for the directory scanning implementation
func scanDir(dir string) *Result{
	results:= Result{dir, 0, 0, 0, 0, 0}
	var walkFunction = func(directoryPath string, info os.FileInfo, err error) error{
		if err != nil {
			fmt.Printf("Error at path: %q", directoryPath)
			return err
		}
		if info.IsDir() {
			results.dirs++
		}
		if info.Mode() & os.ModeSymlink !=0{
			results.symblink++;
		}
		if info.Mode() & os.ModeDevice !=0{
			results.devices++;
		}
		if info.Mode() & os.ModeSocket != 0{
			results.sockets++;
		}else {
			results.misc++;
		}
		return nil
	}
	error:= filepath.Walk(dir, walkFunction)
	if error != nil{
		fmt.Printf("unable to create test dir tree: %v\n", error)
	}
	return &results
}

func main() {

	if len(os.Args) < 2 {
		fmt.Println("Usage: ./dir-scan <directory>")
		os.Exit(1)
	}

	result:=scanDir(os.Args[1])
	fmt.Println("+-------------------------+------+");
	fmt.Println("| Path \t\t\t  |",result.path, "|");
	fmt.Println("+-------------------------+------+");
	fmt.Println("| Directories \t\t  |",result.dirs,"|");
	fmt.Println("| Symlinks \t\t  |",result.symblink,"|");
	fmt.Println("| Devices \t\t  |",result.devices,"|");
	fmt.Println("| Sockets \t\t  |",result.sockets,"|");
	fmt.Println("| Others \t\t  |",result.misc,"|");
	fmt.Println("+-------------------------+------+");
}
