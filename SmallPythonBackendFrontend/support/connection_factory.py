import mysql.connector
from support.Response_Maker import IS_DEBUG
class Connection(object):
    def __init__(self):
        if IS_DEBUG:
            print("JÄVLA OBJEKT: ")
        self.host = "localhost"
        #self.user="idiot"
        self.user="bottled_horse"#Comment this to see when shit goes wrong (makeing user invalid = "idiot").
        self.password="pwd"
        self.database="horse_database_01"
    def make(self):
        """
        Makes connection to database and returns (you should close the connection).
        """        
        try:
            cnx = mysql.connector.connect(user=self.user, password=self.password,host=self.host, database=self.database)
            return cnx
        except:
            return None
    def close(self,con):
        """
        Will only try to close con of type mysql.connector
        that are not None
        """
        """
        if IS_DEBUG:
            print("CON IS TYPE: ",type(con))
            print("CON IS type name: ",type(con).__name__)
        """
        if con == None:
            return
        if type(con).__name__ != "MySQLConnection":
            return
        #ELSE
        try:
            con.close()
        except:
            if IS_DEBUG:
                print("From connection factory: Failed to close con: ",con)
        