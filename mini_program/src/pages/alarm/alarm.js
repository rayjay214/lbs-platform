const app = getApp();
Page({

  /**
   * 页面的初始数据
   */
  data: {
    login_type:'',
    alarmList: [{
      "alarm_type": "",
      "user_name": "",
      "send_time": "",
      "alarm_num":"",
      "alarm_type_id":""
      }],
      alarmTypes: [
        "Vibration",
        "TeleCutoff",
        "LowBattery",
        "SOS",
        "Overspeed",
        "Offline",
        "Enclosure",
        "GeoFence",
        "Move",
        "OverstepLine",
        "Overtime",
        "PassStation",
        "AheadOfSchedule", 
        "OverspeedRoad",
        "Turnround",
        "OfflineMidway",
        "LongTimeStop",
        "AntennaShortCircuit",
        "AntennaOpenCircuit",
        "InBlindArea",
        "OutOfBlindArea",
        "GEOFenceIn",
        "Boot",
        "GPSFirstPositioning",
        "EXBATALM",
        "EXBATCUT",
        "Starting",
        "GeoAreaFence",
        "Remove",
        "Light",
        "Magnet",
        "Tamper",
        "Bluetooth",
        "SignalProhibit",
        "FakeStation",
        "Shield",
        "EnterRiskArea",
        "LeaveRiskArea",
        "RiskAreaLongStay",
        "HighPower",
        "Collision",
        "RapidAcceleration",
        "QuickSlowdown",
        "Overturn",
        "SharpTurn",
        "Open",
      ]

  },

  /**
   * 生命周期函数--监听页面加载
   */
  onLoad: function (options) {
    let that = this
    wx.setNavigationBarColor({
      frontColor: '#ffffff',
      backgroundColor:'#3174EC'
    })
    
  },

  /**
   * 报警信息
   */
  getAlarm: function() {
    let that = this
    const url = 'https://www.yunjiwulian.com/ent/getAlarmByEid'
    let data = {
      eid: app.globalData.eid,
      access_token: app.globalData.access_token,
      map_type: 'amap',
      
    }
    wx.request({
      url: url,
      data:data,
      success: function (res) {
        if (res.data.errcode === 0) {
          let arr = res.data.data
          for (let i = 0; i < arr.length;i++){
            let date=new Date(Number(arr[i].send_time)*1000);
            let Y = date.getFullYear();
            let M = (date.getMonth() + 1 < 10 ? '0' + (date.getMonth() + 1) : date.getMonth() + 1);
            let D = date.getDate() > 9 ? date.getDate() : "0" + date.getDate();
            let h = date.getHours() > 9 ? date.getHours() : "0" + date.getHours();
            let m = date.getMinutes() > 9 ? date.getMinutes() : "0" + date.getMinutes();
            let s = date.getSeconds() > 9 ? date.getSeconds() : "0" + date.getSeconds();
            arr[i].send_time = Y + '-' + M + '-' + D + ' '+ h + ':'+ m + ':' + s;
          }
          that.setData({
            alarmList: arr
          })
        } else {
          console.log(res.data.errcode)
        }
      }
    })

  },

  /**
   * 生命周期函数--监听页面初次渲染完成
   */
  onReady: function () {
    
  },

  /**
   * 生命周期函数--监听页面显示
   */
  onShow: function () {
    this.getAlarm();
  },

  /**
   * 生命周期函数--监听页面隐藏
   */
  onHide: function () {

  },

  /**
   * 生命周期函数--监听页面卸载
   */
  onUnload: function () {

  },

  /**
   * 页面相关事件处理函数--监听用户下拉动作
   */
  onPullDownRefresh: function () {

  },

  /**
   * 页面上拉触底事件的处理函数
   */
  onReachBottom: function () {

  },

  readDetail: function (e) {
    var $data = e.currentTarget.dataset; 
    wx.navigateTo({
      url: './../alarmDetails/alarmDetails?alarm_type=' + $data.alarm_type + "&user_name=" + $data.user_name + "&alarm_type_id=" + $data.alarm_type_id
    })

  },

  /**
   * 用户点击右上角分享
   */
  onShareAppMessage: function () {
    return {
      title: '数化万物 智在融合',
      path: 'pages/authorize/authorize',
      imageUrl: './../images/forward.jpg'
    }
  }
})