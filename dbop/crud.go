package dbop

import (
	"fmt"
	"database/sql"
	_ "github.com/go-sql-driver/mysql"
)

var db *sql.DB
var err error


type User struct {
	Id   int
	Name string
	Age  int
	Money int
}


func InitDB() error {
	//数据库
	//用户名:密码啊@tcp(ip:端口)/数据库的名字
	dsn := "root:123456789@/ku"
	db, err = sql.Open("mysql", dsn)//打开数据集，open不会检验用户名和密码
	if err != nil{
		return fmt.Errorf("dsn:%s invalid", dsn)
	}
	err = db.Ping()//ping数据库看能ping通吗
	if err != nil{
		return fmt.Errorf("open %s faild", dsn)
	}
	fmt.Println("数据库连接成功")
	return nil
}

func Insert(sqlstr string){
	ret, err := db.Exec(sqlstr)//执行sql语句
	if err != nil{
		fmt.Printf("insert faild!")
		return
	}

	//如果是插入数据的操作，能够拿到插入数据的id
	id, err := ret.LastInsertId()
	if err != nil {
		fmt.Println("get id faild!")
		return
	}
	fmt.Println("id", id)
}

func Delete(sqlstr string){
	ret, err := db.Exec(sqlstr)
	if err != nil{
		fmt.Println("delete faild!")
		return
	}
	n, _ := ret.RowsAffected()
	fmt.Printf("删除了%d行数据\n", n)
}


func Update(sqlstr string) {
	ret, err := db.Exec(sqlstr)
	if err != nil {
		fmt.Println("update failed!")
		return
	}
	n, _ := ret.RowsAffected()
	fmt.Printf("更新了%d行数据\n", n)
}

func Query(sqlstr string) []User {
	rows, err := db.Query(sqlstr)
	if err != nil{
		fmt.Println("query faild!")
		return nil
	}

	defer rows.Close()
	var uu []User
	for rows.Next(){
		var u User
		rows.Scan(&u.Name, &u.Age, &u.Money, &u.Id)
		uu = append(uu,u)
	}
	return uu
}
