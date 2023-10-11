
package main
 
import (
	"link/dbop"
	"fmt"
)
 

func main() {
	err := dbop.InitDB()
	if err != nil {
		fmt.Println(err)
		return
	}
	var u []dbop.User
	dbop.Insert("insert into first value ('xiaohou',25, 1313, 4)")
	dbop.Update("update first set name='xiao' where id=1")
	dbop.Delete("delete from first where name='xiaohou'")
	u = dbop.Query("select * from first")
	fmt.Println(len(u))
	for _, ui := range u{
		fmt.Println(ui.Name, ui.Age, ui.Money, ui.Id)
	}
}