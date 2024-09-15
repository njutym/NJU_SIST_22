-- 姓名：田永铭
-- 学号：221900180
-- 提交前请确保本次实验独立完成，若有参考请注明并致谢。

-- ____________________________________________________________________________________________________________________________________________________________________________________________________________
-- BEGIN Q1.1

delimiter <>
drop procedure if exists q1_1 <>
create procedure q1_1(in product_name_to_search_for varchar(40))
begin
		select om.customerNo, c.customerName, om.orderNo,
			od.quantity, od.quantity*od.price as total_price
		from OrderMaster om,OrderDetail od,Customer c,Product p
		where p.productName = product_name_to_search_for and om.orderNo = od.orderNo 
			and om.customerNo = c.customerNo and od.productNo = p.productNo 
		order by total_price desc;
end 
<>
delimiter ;	
call q1_1("32M DRAM");

-- END Q1.1

-- ____________________________________________________________________________________________________________________________________________________________________________________________________________
-- BEGIN Q1.2
delimiter <>
drop procedure if exists q1_2 <>
create procedure q1_2(in employee_no char(8))
begin
	select e.employeeNo, e.employeeName, e.gender,
		   e.hireDate, e.department
    from Employee e
    where e.hireDate < (
		select  e2.hireDate
        from Employee e2
        where e2.employeeNo = employee_no
			and	e2.department = e.department
    );
end 
<>
delimiter ;	
call q1_2("E2008005");
-- END Q1.2

-- ____________________________________________________________________________________________________________________________________________________________________________________________________________
-- BEGIN Q2.1
delimiter <>
drop function if exists q2_1 <>
create function q2_1(product_name varchar(40))
	returns double deterministic
begin	
	declare average double ;
    select sum(od.price*od.quantity)/sum(od.quantity) into average
	from OrderDetail od, Product p
	where p.productName = product_name and p.productNo = od.productNo;
	return average;
end 
<>
delimiter ;	
select distinct p.productName as productName, q2_1(p.productName) as average
from Product p;

-- END Q2.1

-- ____________________________________________________________________________________________________________________________________________________________________________________________________________
-- BEGIN Q2.2
delimiter <>
drop function if exists q2_2 <>
create function q2_2(product_no char(9))
	returns integer deterministic
begin	
	declare total integer ;
    select sum(od.quantity) into total
	from OrderDetail od
	where od.productNo = product_no;
	return total;
end 
<>
delimiter ;	
select p.productNo, p.productName, 
		q2_2(p.productNo) as quantity
from Product p
where q2_2(p.productNo) > 4;

-- END Q2.2

-- ____________________________________________________________________________________________________________________________________________________________________________________________________________
-- BEGIN Q3.1
delimiter <>
drop trigger if exists q3_1 <>
create trigger q3_1 
	before insert on Product
	for each row
begin
	if new.productPrice > 1000 
		then set new.productPrice = 1000;
	end if ;
end 
<>
delimiter ;

select p.productNo as productNo,p.productName as productName,
p.productPrice as productPrice
from Product p;

-- END Q3.1

-- ____________________________________________________________________________________________________________________________________________________________________________________________________________
-- BEGIN Q3.2
delimiter <>
drop trigger if exists q3_2 <>
create trigger q3_2
	after insert on OrderMaster
	for each row
begin
	declare hire_year int;
    select YEAR(hireDate) into hire_year from Employee where employeeNo = new.employeeNo;
	if hire_year >= '1992' 
		then
			update Employee set salary = salary * 1.05 where employeeNo = new.employeeNo;
		else
			update Employee set salary = salary * 1.08 where employeeNo = new.employeeNo;
    end if;
end 
<>
delimiter ;

select e.employeeNo,e.employeeName,e.salary
from Employee e;

-- END Q3.2

