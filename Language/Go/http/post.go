package post

import (
    "fmt"
    "strings"
    "io/ioutil"
    "net/http"
)

func Post(uri string, data map[string]string) (string, error) {
    var req http.Request
    var err error
    req.ParseForm()
    for k, v := range data {
        req.Form.Add(k, v)
    }
    body := strings.NewReader(req.Form.Encode())
    resp, err :=  http.Post(uri, "application/x-www-form-urlencoded", body)
    if err != nil {
        fmt.Printf("response error%d", err)
        return "", err
    }
    respBody, _ := ioutil.ReadAll(resp.Body)
    return string(respBody), err
}