1. !! You MUST run this testing program in trading time !!
2. make sure the price can NOT make a deal
3. program usage: speed.exe <config file path>
       for example: speed.exe .\speed-win.json
4. the result file will save into 'result.csv' in the logfile path
5. configuration file:

{
  # logfile path
  "logpath": ".\\mylog.txt",
  # interval time in milliseconds (ms) between two orders
  "interval": 1000,
  # loop times for get average time
  "loop_times": 5,

  # the array of ctp interfaces
  "interface": [
    {
      # name of cpt interface
      "name": "ctp 1",
      # broker id for your broker company
      "brokerid": "9999",
      # your account
      "clientid": "068576",
      # your password
      "password": "simnow",
      # your link is udp or tcp
      "udp": false,
      # the link uri address
      "tduri": "tcp://180.168.146.187:10000"
    },
    {
      "name": "ctp 2",
      "brokerid": "9999",
      "clientid": "068576",
      "password": "simnow",
      "udp": false,
      "tduri": "tcp://180.168.146.187:10000"
    },
    {
      "name": "ctp 3",
      "brokerid": "9999",
      "clientid": "068576",
      "password": "simnow",
      "udp": false,
      "tduri": "tcp://180.168.146.187:10000"
    }
  ],

  # test-case array of orders
  "test_case": [
    { # an order
      # exchange name
      "name": "ZhengZhou",
      # instrument code
      "instrument": "CF801",
      # price
      "price": 16485,
      # diraction, 1 for short, 0 for long
      "dir": 1,
      # volume
      "volume": 1
    },
    {
      "name": "DaLian",
      "instrument": "j1801",
      "price": 2536.5,
      "dir": 1,
      "volume": 4
    },
    {
      "name": "ShangHai",
      "instrument": "rb1801",
      "price": 3811,
      "dir": 0,
      "volume": 10
    },
    {
      "name": "Zhongjin",
      "instrument": "IF1803",
      "price": 3407.8,
      "dir": 0,
      "volume": 3
    }
  ]
}
