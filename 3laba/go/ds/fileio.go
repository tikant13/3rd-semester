package ds

import (
	"bufio"
	"encoding/binary"
	"errors"
	"fmt"
	"io"
	"os"
)

// Общие функции для сериализации. Сами методы SaveText/LoadText/SaveBinary/LoadBinary
// находятся у каждой структуры. Функции с маленькой буквы не видны вне пакета (private).

// errBadFile — файл открылся, но его содержимое не подходит.
var errBadFile = errors.New("файл повреждён")

// saveFile создаёт файл и передаёт буфер функции write.
// bufio.Writer запоминает первую ошибку записи, Flush её возвращает.
func saveFile(fileName string, write func(w *bufio.Writer)) error {
	file, err := os.Create(fileName)
	if err != nil {
		return err
	}
	w := bufio.NewWriter(file)
	write(w)
	if err := w.Flush(); err != nil {
		_ = file.Close()
		return err
	}
	return file.Close()
}

// loadFile открывает файл и передаёт его функции read.
func loadFile(fileName string, read func(r *bufio.Reader) error) error {
	file, err := os.Open(fileName)
	if err != nil {
		return err
	}
	err = read(bufio.NewReader(file))
	_ = file.Close() // файл только читали, ошибка закрытия не важна
	return err
}

// writeLine пишет значения через пробел и перевод строки (для текстового формата).
func writeLine(w io.Writer, values ...any) {
	_, _ = fmt.Fprintln(w, values...)
}

// readWords читает все слова текстового файла (разделители — пробелы и переводы строк).
func readWords(r io.Reader) []string {
	scanner := bufio.NewScanner(r)
	scanner.Split(bufio.ScanWords)
	var words []string
	for scanner.Scan() {
		words = append(words, scanner.Text())
	}
	return words
}

// В бинарном файле число занимает 4 байта (int32, младший байт первым),
// как int в C++-версии на x86-64.
func writeInt(w io.Writer, value int) {
	_ = binary.Write(w, binary.LittleEndian, int32(value))
}

func readInt(r io.Reader) (int, error) {
	var value int32
	err := binary.Read(r, binary.LittleEndian, &value)
	return int(value), err
}

// readCount читает количество элементов: отрицательное значит, что файл повреждён.
func readCount(r io.Reader) (int, error) {
	count, err := readInt(r)
	if err != nil {
		return 0, err
	}
	if count < 0 {
		return 0, errBadFile
	}
	return count, nil
}

// Строка записывается как длина, затем её байты.
func writeString(w io.Writer, value string) {
	writeInt(w, len(value))
	_, _ = io.WriteString(w, value)
}

func readString(r io.Reader) (string, error) {
	length, err := readCount(r)
	if err != nil {
		return "", err
	}
	buf := make([]byte, length)
	if _, err := io.ReadFull(r, buf); err != nil {
		return "", err
	}
	return string(buf), nil
}
