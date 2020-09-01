#include <stdio.h>
#include "mysql.h"
#include <string.h>

#pragma comment(lib, "libmysql.lib")

const char* host = "localhost";
const char* user = "root";
const char* pw = "mysql";
const char* db = "project";
int state;

int main(void) {
	MYSQL* connection = NULL;
	MYSQL conn;
	MYSQL_RES* sql_result;
	MYSQL_ROW sql_row;

	if (mysql_init(&conn) == NULL)
		printf("mysql_init() error!");

	connection = mysql_real_connect(&conn, host, user, pw, db, 3306, (const char*)NULL, 0);
	if (connection == NULL)
	{
		printf("%d ERROR : %s\n", mysql_errno(&conn), mysql_error(&conn));
		return 1;
	}

	else
	{
		printf("Connection Succeed\n"); //if connection is made successfull

		if (mysql_select_db(&conn, db))
		{
			printf("%d ERROR : %s\n", mysql_errno(&conn), mysql_error(&conn));
			return 1;
		}

		FILE* fp = fopen("20170795_1.txt", "r");	//open txt that creates relations
		if (fp == NULL) {
			fprintf(stdout, "FILE OPEN ERROR\n");
			return 0;
		}
		char s[1001];
		char query0[100000];
		while (fgets(s, 1001, fp) != NULL) {		//process query
			strncpy(query0, s, sizeof(s));
			state = mysql_query(connection, query0);
		}
		fp = fopen("20170795_2.txt", "r");		//insert values
		if (fp == NULL) {
			fprintf(stdout, "FILE OPEN ERROR\n");
			return 0;
		}
		while (fgets(s, 1001, fp) != NULL) {
			strncpy(query0, s, sizeof(s));
			state = mysql_query(connection, query0);
		}
		while (1) {
			printf("\n------- SELECT QUERY TYPES -------\n\n");		//main frame
			printf("\t1. TYPE I\n");
			printf("\t2. TYPE II\n");
			printf("\t3. TYPE III\n");
			printf("\t4. TYPE IV\n");
			printf("\t5. TYPE V\n");
			printf("\t0. QUIT\n");
			printf("Which type of query? ");
			int n;
			scanf("%d", &n);		//get the query type from input
			if (n == 1) {
				while (1) {
					printf("\n------- Subtypes in TYPE I -------\n\n");	//type 1 has 3 subtypes
					printf("\t0. QUIT\n");
					printf("\t1. TYPE I-1\n");
					printf("\t2. TYPE I-2\n");
					printf("\t3. TYPE I-3\n");
					printf("----------------------------------\n");
					printf("Select the query type : ");
					int k; scanf("%d", &k);
					printf("\n");
					if (k == 0) break;
					if (k == 1) {
						printf("**Find all customers who had a package on the truck at the time of the crash**\n");
						char query[1000] = "Select distinct CustomerName\
			from Customer, Package, DeliveredBy, Vehicle, Tracking, Recipient\
		where Customer.CustomerID = Package.CustomerID and Package.TrackID = Tracking.TrackID and Package.TrackTime = Tracking.TrackTime and\
			Tracking.TrackID = DeliveredBy.TrackID and Tracking.TrackTime = DeliveredBy.TrackTime\
			and DeliveredBy.VehicleID = Vehicle.VehicleID and Vehicle.VehicleID = \"1721\"and Package.RecipientID = Recipient.RecipientID and Customer.CustomerAddress != Recipient.RecipientAddress";
						int state11 = mysql_query(connection, query);		//store the query in string and process it 
						if (state11 == 0) {
							printf("\n---Customer Name---\n");
							sql_result = mysql_store_result(connection);
							while ((sql_row = mysql_fetch_row(sql_result)) != NULL)		//fetch it line by line
							{
								printf("%s\n", sql_row[0]);		//print the result on console
							}
							printf("\n");
							mysql_free_result(sql_result);
						}
					}
					else if (k == 2) {
						printf("**Find all recipients who had a package on that truck at the time of the crash.**\n");
						char query[1000] = "Select distinct RecipientName\
					from Customer, Package, DeliveredBy, Vehicle, Tracking, Recipient\
				where Customer.CustomerID = Package.CustomerID and Package.TrackID = Tracking.TrackID and Package.TrackTime = Tracking.TrackTime and\
					Tracking.TrackID = DeliveredBy.TrackID and Tracking.TrackTime = DeliveredBy.TrackTime\
					and DeliveredBy.VehicleID = Vehicle.VehicleID and Vehicle.VehicleID = \"1721\" and Package.RecipientID = Recipient.RecipientID and Tracking.Location != Recipient.RecipientAddress";
						int state11 = mysql_query(connection, query);
						if (state11 == 0) {
							printf("---Recipient---\n");
							sql_result = mysql_store_result(connection);
							while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
							{
								printf("%s\n", sql_row[0]);
							}
							printf("\n");
							mysql_free_result(sql_result);
						}
					}
					else if (k == 3) {
						printf("**Find the last successful delivery by that truck prior to the crash**\n");
						char query[1000] = "with lastDelivery(deliveryTime) as (\
						select max(Tracking.TrackTime)\
						from Customer, Package, DeliveredBy, Vehicle, Tracking, Recipient\
					where Customer.CustomerID = Package.CustomerID and Package.TrackID = Tracking.TrackID and Package.TrackTime = Tracking.TrackTime and\
						Tracking.TrackID = DeliveredBy.TrackID and Tracking.TrackTime = DeliveredBy.TrackTime\
						and DeliveredBy.VehicleID = Vehicle.VehicleID and Vehicle.VehicleID = \"1721\" and Package.RecipientID = Recipient.RecipientID\
						and Recipient.RecipientAddress = Tracking.Location)\
						select TrackID\
						from Tracking, lastDelivery\
					where Tracking.TrackTime = lastDelivery.deliveryTime";
						int state11 = mysql_query(connection, query);					//store the query in string
						if (state11 == 0) {
							sql_result = mysql_store_result(connection);
							while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
							{
								printf("Last Successful Delivery is %s\n", sql_row[0]);
							}
							printf("\n");
							mysql_free_result(sql_result);
						}
					}
				}
			}
			else if (n == 2) {
				while (1) {
					printf("**Find the customer who has shipped the most packages in the past certain year.**\n");	
					char year[11];
					printf("Which year? ");				//get the year value
					scanf("%s", year);
					if (!strcmp(year, "0")) break;		//if 0, break to the main frame
					char query[1000] = "with bestBuyer(amount, customerBuyer) as(\
			select count(distinct packageID), Customer.CustomerName\
			from Customer, Package\
		where Customer.CustomerID = Package.CustomerID and timeYear = ";
					strcat(query, year);
					char query2[100] = "	) select max(amount), customerBuyer from bestBuyer";		//concatenate the string
					strcat(query, query2);
					int state11 = mysql_query(connection, query);
					if (state11 == 0) {						//if the connection is made
						sql_result = mysql_store_result(connection);
						bool flag = true;				//flag to let us know if the result is NULL or not
						while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
						{
							if (sql_row[1] == NULL) {
								flag = false;
								break;
							}
							printf("<%s> has shipped the most packages that year \n\n", sql_row[1]);
						}
						if (flag == false) {
							printf("No one has shipped the packages that year\n\n");			//when there is NULL result
						}
						mysql_free_result(sql_result);
					}
				}
			}
			else if (n == 3) {
				while (1) {
					char year[11];
					printf("** Find the customer who has spent the most money on shipping in the past certain year.**\n");
					printf("Which Year? ");
					scanf("%s", year);
					if (!strcmp(year, "0"))break;				//loop until it is not 0
					char query[200] = "select Customer.CustomerName, max(Bill.Amount)\
				from Customer, Bill\
			where Customer.CustomerID = Bill.CustomerID and Bill.BillDate >= ";				//BillDate>=20190101 ~ BillDate<=20191231
					strcat(query, year);
					char mid[100] = "0101 and Bill.BillDate <= ";
					strcat(query, mid);
					strcat(query, year);
					char end[100] = "1231";
					strcat(query, end);
					int state11 = mysql_query(connection, query);
					if (state11 == 0) {
						sql_result = mysql_store_result(connection);
						bool flag = false;
						while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
						{
							if (sql_row[1] == NULL) {
								flag = false;
								break;
							}
							flag = true;
							printf("Customer Name : %s\n", sql_row[0]);
						}
						if (flag == false) {
							printf("No one has bought anything during the year\n");			//when there is no customer Record
							continue;
						}
						mysql_free_result(sql_result);
					}
				}
			}
			else if (n == 4) {
			int k;
				while (1) {
					printf("Find those packages that were not delivered within the promised time\n");
					printf("IF you want to skip to main frame, press 0 : ");			//give out the result until input is 0
					scanf("%d", &k);
					if (k == 0) break;
					const char* query = "select PackageID from Package where Timeliness = 0";
					int state11 = mysql_query(connection, query);
					if (state11 == 0) {
						sql_result = mysql_store_result(connection);
						while ((sql_row = mysql_fetch_row(sql_result)) != NULL)		//fetch the row
						{
							printf("%s\n", sql_row[0]);				//print it in the console
						}
						mysql_free_result(sql_result);
					}
				}
			}
			else if (n == 5) {
			int sss;
			while (1) {
				printf("If you want to quit, press 0 : ");
				scanf("%d", &sss);
				if (sss == 0) break;
				char year[5];
				FILE* sp = fopen("20170795_4.txt", "w");
				printf("Generate the bill for each customer for the past certain month.\n");
				printf("Customer Name : ");
				while (getchar() != '\n');
				char tmp[200];
				fgets(tmp, 200, stdin);
				printf("Which year? ");
				fscanf(stdin, "%s", year);
				tmp[strlen(tmp) - 1] = '\0';
				printf("Generating Bill..\n");
				printf("Generation Completed\n");
				printf("Please check 20170795_4.txt\n");
				printf("\n");
				char query[1000] = "select CustomerName, CustomerAddress, BillNo, amount, ServiceType, packageID, ProductType, PackageType, sum(amount), count(BillNo)\
				from Bill, Customer, Package\
			where Bill.CustomerID = Customer.CustomerID and Customer.CustomerID = Package.CustomerID and CustomerName = \"";
				strcat(query, tmp);
				char tt[30] = "\" and timeYear = \"";
				strcat(query, tt);
				strcat(query, year);
				char ttt[100] = "\"group by amount";
				strcat(query, ttt);
				int state11 = mysql_query(connection, query);
				if (state11 == 0) {
					sql_result = mysql_store_result(connection);
					sql_row = mysql_fetch_row(sql_result);
					fprintf(sp, "Customer Name\t Address\t Amount\n");
					fprintf(sp, "%s\t%s\t%s\n", sql_row[0], sql_row[1], sql_row[8]);
					fprintf(sp, "----------------------------------------------------------------------------------------\n");
					fprintf(sp, "Itemized Billing List\n");
					fprintf(sp, "========================================================================================\n");
					fprintf(sp, "Bill No | PackageID | Price | Product Type | Package Type | Service Type | Number Bought\n");
					fprintf(sp, "|\t%s\t|\t%s\t|\t\t$%s\t\t|\t\t%s\t\t|\t%s\t|\t%s\t|\t%s\t|\n", sql_row[2], sql_row[5], sql_row[3], sql_row[7], sql_row[6], sql_row[4], sql_row[9]);
					fprintf(sp, "----------------------------------------------------------------------------------------\n");
					fclose(sp);
					mysql_free_result(sql_result);
				}
			}
			}
			else if (n == 0) {
			FILE* dropTxt = fopen("20170795_3.txt", "r");
			char dropTable[100000];
			while (fgets(s, 1001, dropTxt) != NULL) {
				strncpy(dropTable, s, sizeof(s));
				int state2 = mysql_query(connection, dropTable);
			}
			mysql_close(connection);
			printf("HAVE A GOOD DAY :)");
			break;
			}
		}
	}
	return 0;
}
