package main

import (
	"encoding/csv"
	"encoding/json"
	"encoding/xml"
	"flag"
	"fmt"
	"io"
	"net/http"
	"log"
	"os"
	"slices"
	"strconv"
	"strings"
)

type JSON struct {
    Facts []string      `json:"data"`
}

type XML struct {
    XMLName xml.Name    `xml:"facts"`
    Facts []string      `json:"data" xml:"fact"`
}

const ENDPOINT = "https://meowfacts.herokuapp.com"
const FILES_DIR = "files"
const CSV_HEADER string = "id,fact"

var format string
var number int

func GetBody(url string) []byte {
    resp, err := http.Get(url)
    if err != nil {
        log.Fatal(err)
    }
    defer resp.Body.Close()

    body, err := io.ReadAll(resp.Body)
    if err != nil {
        log.Fatal(err)
    }

    return body
}

func ParseBody(body []byte) []string {
    str := string(body)

    head := `{"data": [`
    tail := "]}"

    start := strings.Index(str, head) + len(head)
    end := strings.LastIndex(str, tail)

    facts := str[start:end]
    facts = strings.Trim(facts, `"`)

    return strings.Split(facts, `","`)
}

func OpenFile(ext string) *os.File {
    if err := os.MkdirAll(FILES_DIR, 0755); err != nil {
        log.Fatal(err)
    }

    path := FILES_DIR + "/facts." + ext

    file, err := os.OpenFile(path, os.O_RDWR|os.O_APPEND|os.O_CREATE, 0644)
    if err != nil {
        log.Fatal(err)
    }

    return file
}

func RemoveDuplicates(prev []string, curr []string) []string {
    var result []string
    for _, el := range curr {
        if !slices.Contains(prev, el) {
            result = append(result, el)
        }
    }

    return result
}

func NoWrite(body []byte) error {
    arr := ParseBody(body)

    for idx, el := range arr {
        fmt.Printf("%d: %s\n", idx, el)
    }

    return nil
}

func WriteTxt(body []byte) error {
    file := OpenFile(format)
    defer file.Close()

    prevBytes, err := os.ReadFile(file.Name())
    if err != nil {
        log.Fatal(err)
    }

    previous := strings.Split(string(prevBytes), "\n")
    current := ParseBody(body)

    result := RemoveDuplicates(previous, current)
    result = append(previous, result...)

    if result[0] == "" {
        result = result[1:]
    }

    bytes := []byte(strings.Join(result, "\n"))

    return os.WriteFile(file.Name(), bytes, 0644)
}

func WriteCsv(body []byte) error {
    file := OpenFile(format)
    defer file.Close()

    prevBytes, err := os.ReadFile(file.Name())
    if err != nil {
        log.Fatal(err)
    }

    var prevCsv [][]string
    if len(prevBytes) > 0 {
        prevCsv, err = csv.NewReader(file).ReadAll()
        if err != nil {
            log.Fatal("Cannot unmarshal previous data:", err)
        }
    }

    var previous []string
    if len(prevCsv) > 1 {
        for _, row := range prevCsv[1:] {
            previous = append(previous, strings.Join(row, ","))
        } 
    }

    current := ParseBody(body)
    result := RemoveDuplicates(previous, current)   // This really has no effect

    if len(result) == 0 {
        log.Println("No new facts were discovered")
        return nil
    }

    if repeated := number % len(result); repeated > 0 {
        log.Printf("There were %d/%d repeated facts\n", repeated, number)
    }

    var lastId int
    if len(previous) > 0 {
        row := previous[len(previous)-1]
        idStr := strings.SplitN(row, ",", 2)[0]

        lastId, err = strconv.Atoi(idStr)
        if err != nil {
            log.Fatal(err)
        }
    }

    lastId += 1

    for idx, el := range previous {
        row := strings.Replace(el, ",", `,"`, 1) + `"`
        previous[idx] = row
    }

    for idx, el := range result {
        el = strings.Replace(el, `"`,  `'`, 1)
        row := fmt.Sprintf(`%d,"%s"`, idx + lastId, el)
        result[idx] = row
    }

    result = append(previous, result...)

    if result[0] != CSV_HEADER {
        result = append([]string{CSV_HEADER}, result...)
    }

    bytes := []byte(strings.Join(result, "\n"))

    return os.WriteFile(file.Name(), bytes, 0644)
}

func WriteXml(body []byte) error {
    file := OpenFile(format)
    defer file.Close()

    prevBytes, err := os.ReadFile(file.Name())
    if err != nil {
        log.Fatal(err)
    }

    var previous XML
    if len(prevBytes) > 0 {
        if err := xml.Unmarshal(prevBytes, &previous); err != nil {
            log.Fatal("Cannot unmarshal previous data:", err)
        }
    }

    var current XML
    if err := json.Unmarshal(body, &current); err != nil {
        log.Fatal("Cannot unmarshal XML:", err)
    }

    result := RemoveDuplicates(previous.Facts, current.Facts)

    if len(result) == 0 {
        log.Println("No new facts were discovered")
        return nil
    }

    if repeated := number % len(result); repeated > 0 {
        log.Printf("There were %d/%d repeated facts\n", repeated, number)
    }

    current.Facts = append(previous.Facts, result...)

    bytes, err := xml.MarshalIndent(current, "", "  ")
    if err != nil {
        log.Fatal(err)
    }

    bytes = append([]byte(xml.Header), bytes...)

    return os.WriteFile(file.Name(), bytes, 0644)
}

func WriteJson(body []byte) error {
    file := OpenFile(format)
    defer file.Close()

    prevBytes, err := os.ReadFile(file.Name())
    if err != nil {
        log.Fatal(err)
    }

    var previous JSON
    if len(prevBytes) > 0 {
        if err := json.Unmarshal(prevBytes, &previous); err != nil {
            log.Fatal("Cannot unmarshal previous data:", err)
        }
    }

    var current JSON
    if err := json.Unmarshal(body, &current); err != nil {
        log.Fatal("Cannot unmarshal JSON:", err)
    }

    result := RemoveDuplicates(previous.Facts, current.Facts)

    if len(result) == 0 {
        log.Println("No new facts were discovered")
        return nil
    }

    if repeated := number % len(result); repeated > 0 {
        log.Printf("There were %d/%d repeated facts\n", repeated, number)
    }

    current.Facts = append(previous.Facts, result...)

    bytes, err := json.MarshalIndent(current, "", "    ")
    if err != nil {
        log.Fatal(err)
    }

    return os.WriteFile(file.Name(), bytes, 0644)
}

func main() {
    arg_number := flag.Int("number", 1, "Number of facts to request")
    arg_format := flag.String("format", "none", "File format: txt, csv, json, xml or blank")
    flag.Parse()

    number = *arg_number
    format = *arg_format

    if number < 0 {
        log.Fatal("number must be a positive integer")
    }

    var writers = map[string]func([]byte) error {
        "none": NoWrite,
        "txt":  WriteTxt,
        "csv":  WriteCsv,
        "xml":  WriteXml,
        "json": WriteJson,
    }

    writer, ok := writers[format]
    if !ok {
        log.Fatal("Unknown format")
    }

    url := fmt.Sprintf("%s/?count=%d", ENDPOINT, number)
    body := GetBody(url)

    if err := writer(body); err != nil {
        log.Fatal(err)
    }

    if format != "none" {
        log.Printf("Finished writing %s file\n", format)
    }
}
