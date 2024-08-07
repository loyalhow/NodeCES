import streamlit as st
import mysql.connector
import pandas as pd
from datetime import datetime
import matplotlib.pyplot as plt
import seaborn as sns
from io import BytesIO

# 设置图表背景为透明
plt.rcParams['axes.facecolor'] = 'none'
plt.rcParams['figure.facecolor'] = 'none'
plt.rcParams['savefig.facecolor'] = 'none'
plt.rcParams['legend.facecolor'] = 'none'


# Streamlit 应用
st.title('SCD4x & SCD30 Visualization')



# 配置数据库连接
db_config = {
    'user': 'fudan',  # 替换为你的数据库用户名
    'password': '',  # 替换为你的数据库密码
    'host': 'localhost',
    'database': 'fudan'
}

# 连接到数据库
db_connection = mysql.connector.connect(**db_config)

# 获取所有唯一的传感器编号
query_id = "SELECT DISTINCT id FROM scd4x_scd30"
cursor = db_connection.cursor()
cursor.execute(query_id)
sensor_ids = cursor.fetchall()
sensor_ids = [id[0] for id in sensor_ids]  # 提取编号列表

default_sensor_ids = [0, 1, 2, 3, 4]  # 默认选择传感器ID为1、2和3
selected_sensor_ids = st.multiselect('Select Sensor IDs', sensor_ids, default=default_sensor_ids)

# 关闭游标
cursor.close()

# 动态生成SQL查询
query_datetime = f"""
SELECT datetime
FROM scd4x_scd30
WHERE id IN {tuple(selected_sensor_ids)}
ORDER BY datetime DESC
"""

# 执行SQL查询
df_datetime = pd.read_sql_query(query_datetime, db_connection)
datetime_value_min = df_datetime['datetime'].min().timestamp()
datetime_value_max = df_datetime['datetime'].max().timestamp()

# 使用 st.session_state 存储滑动条的值
if 'datetime_value' not in st.session_state:
    st.session_state.datetime_value = [datetime_value_min, datetime_value_max]


# 使用 st.slider 创建一个滑动条来选择起始时间
datetime_value = st.slider('Start Start and End Time', min_value=st.session_state.datetime_value[0], max_value=st.session_state.datetime_value[1], step=10.0, value=st.session_state.datetime_value)

start_datetime = datetime.fromtimestamp(datetime_value[0])
end_datetime = datetime.fromtimestamp(datetime_value[1])

# 根据选择的传感器编号查询数据
query = f"""
SELECT id, temperature, humidity, CO2, datetime
FROM scd4x_scd30
WHERE id IN {tuple(selected_sensor_ids)}
AND datetime BETWEEN '{start_datetime}' AND '{end_datetime}'
ORDER BY datetime DESC  # 使用 datetime 列排序
"""
# 执行SQL查询
df = pd.read_sql_query(query, db_connection)

# 创建一个下载按钮，允许用户下载 DataFrame 和图表
def download_data():
    csv = df.to_csv(index=False)
    return csv

st.download_button(
    label="Download DataFrame as CSV",
    data=download_data(),
    file_name="scd4x_scd30_trial_data.csv",
    mime='text/csv'
)


# 绘制图表
if not df.empty:
    fig, ax = plt.subplots(3, 1, figsize=(10, 15))

    palette = sns.color_palette("colorblind", len(selected_sensor_ids))

    for sensor_id in selected_sensor_ids:
        sensor_data = df[df['id'] == sensor_id]
        sensor_data.plot(x='datetime', y='CO2', kind='scatter', s=3, c=palette[sensor_id], ax=ax[0], label=f'Sensor {sensor_id}')
        sensor_data.plot(x='datetime', y='temperature', kind='scatter', s=3, c=palette[sensor_id], ax=ax[1], label=f'Sensor {sensor_id}')
        sensor_data.plot(x='datetime', y='humidity', kind='scatter', c=palette[sensor_id], s=3, ax=ax[2], label=f'Sensor {sensor_id}')


    # 设置 x 轴标签的角度，使其更可读
    plt.setp(ax[0].get_xticklabels(), rotation=45, color='w')
    plt.setp(ax[1].get_xticklabels(), rotation=45, color='w')
    plt.setp(ax[2].get_xticklabels(), rotation=45, color='w')

    plt.setp(ax[0].get_yticklabels(), color='w')
    plt.setp(ax[1].get_yticklabels(), color='w')
    plt.setp(ax[2].get_yticklabels(), color='w')


    # 设置 x 轴范围
    ax[0].set_xlim(start_datetime, end_datetime)
    ax[1].set_xlim(start_datetime, end_datetime)
    ax[2].set_xlim(start_datetime, end_datetime)

    
    ax[0].set_xlabel('Time',color='w')
    ax[0].set_ylabel('CO2 conc. [ppm]',color='w')
    ax[0].legend(loc='lower right', facecolor='white', edgecolor='white')

    ax[1].set_xlabel('Time',color='w')
    ax[1].set_ylabel('Temperature [C]',color='w')
    ax[1].legend(loc='lower right', facecolor='white', edgecolor='white')

    ax[2].set_xlabel('Time',color='white')
    ax[2].set_ylabel('Humidity [%]',color='w')
    ax[2].legend(loc='lower right', facecolor='white', edgecolor='white')

    # 设置坐标轴线颜色为白色
    ax[0].spines['top'].set_color('white')
    ax[0].spines['right'].set_color('white')
    ax[0].spines['bottom'].set_color('white')
    ax[0].spines['left'].set_color('white')

    # 设置坐标轴线颜色为白色
    ax[1].spines['top'].set_color('white')
    ax[1].spines['right'].set_color('white')
    ax[1].spines['bottom'].set_color('white')
    ax[1].spines['left'].set_color('white')

    # 设置坐标轴线颜色为白色
    ax[2].spines['top'].set_color('white')
    ax[2].spines['right'].set_color('white')
    ax[2].spines['bottom'].set_color('white')
    ax[2].spines['left'].set_color('white')

    st.pyplot(fig)


# 关闭数据库连接
db_connection.close()
