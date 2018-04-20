package main

import (
	"encoding/json"
)

type LocationStruct struct {
	Left	int
	Top		int
	Width	int
	Height	int
}

type ResultStruct struct {
	Type string
	Probability float64
	Location LocationStruct
}

type ResponseStruct struct {
	Error_msg string
	Error_code int
	Log_id int64
	Result_num int
	Result []ResultStruct
}

func main() {
	/* go string to json */
	// 定义数据结构
	var resData ResponseStruct
	res := ""
	if err := json.Unmarshal([]byte(res), &resData); err != nil {
		// do
	}
}