const app = getApp();
Page({

  /**
   * 页面的初始数据
   */
  data: {
    deviceList: [],
    statusMap: {
      'online':'在线',
      'offline':'离线',
      'static':'静止'
    },
  },

  /**
   * 生命周期函数--监听页面加载
   */
  onLoad: function (options) {
    this.getDeviceList()
    wx.setNavigationBarColor({
      frontColor: '#ffffff',
      backgroundColor: '#3174EC'
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

  /**
   * 用户点击右上角分享
   */
  onShareAppMessage: function () {
    return {
      title: '数化万物 智在融合',
      path: 'pages/authorize/authorize',
      imageUrl: './../../images/forward.jpg'
    }
  },
  getDeviceList: function () {
    const that = this;
    wx.request({
      url: 'https://www.yunjiwulian.com/ent/getRunInfoByEid',
      data: {
        map_type: 'amap',
        eid: app.globalData.eid,
        access_token: app.globalData.access_token
      },
      success: function (res) {
        that.setData({
          deviceList: res.data.data
        })
      },
      fail: function (err) {
        console.log(err)
      }
    })
  },
  toDetail (e) {
    var dev_id = e.target.dataset.id;
    var heart_time = e.target.dataset.htime;
    wx.navigateTo({
      url: './../detail/detail?dev_id=' + dev_id + '&heart_time='+ heart_time
    })
  },
  chooseDevice (e) {
    var dev_id = e.target.dataset.id;
    app.globalData.dev_id = dev_id
    wx.reLaunch({
      url: './../monitor/monitor'
    })
  }
})