#STG_thedc17

#这是预审(Nov.7)用的代码

#开工啦！！！！！！```决赛倒计时 7 天```

#修改的速度防止溢出代码
<pre><code>    //调节占空比控制电机速度,可供上层函数调用
    void motor_setSpeed(uint32_t speed) {
      TIM1->CCR1= speed > 1000 ? 1000 : speed;
      TIM1->CCR2= speed > 1000 ? 1000 : speed;
    }
    
    void motor_setLeftSpeed(uint32_t speed) {
      TIM1->CCR1 = speed > 1000 ? 1000 : speed;
    }
    void motor_setRightSpeed(uint32_t speed) {
      TIM1->CCR2 = speed > 1000 ? 1000 : speed;
    }</code></pre>

#修改的状态机预审版本代码
<pre><code>    void TIM2_IRQHandler(void){
      //TIM_ClearFlag
        
    	static char if_turn_l = 0, if_turn_r = 0, if_backward = 0;
    	static int counter_l = 0, counter_r = 0, counter_b = 0;
    	static char debug = 0;
    	
    	if(ReceiveAI.Status != 0x01)
    	{
    		motor_sleep();
    	}
    	else
    	{
    		motor_wake();
    		
    		//turn right to get prop
    		if(debug == 0)
    		{
    			motor_turn_right(-600,1000); //1500 is ok
    			Delay(0xFFFF00);
    			motor_forward();
    			motor_setSpeed(800);
    			Delay(0xFFFF00);
    			debug++;
    		}
    	}
    
    	
    	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
    	{
    			if(if_turn_l == 1 && !GPIOD->IDR&GPIO_Pin_8)
    			{
    				counter_l++;
    				if(counter_l >= 14000)
    				{
    					if_turn_l = 0;
    					counter_l = 0;
    				}
    			}
    			else if(if_turn_r == 1 && !GPIOD->IDR&GPIO_Pin_9)
    			{
    				counter_r++;
    				if(counter_r >= 14000)
    				{
    					if_turn_r = 0;
    					counter_r = 0;
    				}
    			}
    			else if(if_backward == 1) //May cause problem. Ban reverse runnning! - by haldak
    			{
    				counter_b++;
    				if(counter_b == 15000)
    				{
    					motor_turn_left(-500,1000); //1600 is ok
    				}
    				if(counter_b >= 30000)
    				{
    					counter_b = 0;
    					if_backward = 0;
    		  	}
    	  	}
    		
          if(GPIOD->IDR&GPIO_Pin_8 && GPIOD->IDR&GPIO_Pin_9) //两边碰线
    			{
            //USART_SendData(USART2,'a');
    				motor_setSpeed(1000); //800 is ok
    				if_backward = 1;
    				motor_backward();
          }
            
          else if(GPIOD->IDR&GPIO_Pin_8) //左边碰线
    			{ 
     				//USART_SendData(USART2,'b');
    				if_turn_r = 1;
    				motor_turn_right(-600,1000); //1500 is ok
          }
       
          else if(GPIOD->IDR&GPIO_Pin_9) //右边碰线
    			{ 
            //USART_SendData(USART2,'c');
    				if_turn_l = 1;
            motor_turn_left(-600,1000); //1300 is ok
          }
            
          else //没有碰线
    			{ 
            //USART_SendData(USART2,'d');
    				motor_setSpeed(800); //800 is ok
    				motor_forward();
          }
      }
      TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    }</code></pre>

**master分支已经维护，禁止使用git push --force！**

Updated by haldak, 11/7/2015, 00:54am

大家加油呀~

.md文件支持Markdown语法，有需要可以参考这个：http://wowubuntu.com/markdown/#list

记得在账单中添加各种支出~

## 比赛时间
- **决赛 12.05(第12周周六)**
- //预审 11.7(第8周周六)
- //初赛 11.22(第10周周日)

## 决赛任务
- [x] 围观初赛，确认部分对手实力
- [x] 11.20(Fri), 新的pcb
- [ ] 加入惯导
- [ ] 地图信息处理AI（i.e. 储存地图信息，获取道具策略，进攻对方策略）
- [ ] 进攻AI与防御AI
- [ ] 黑科技干扰对方
- [ ] **摄像头代替红外方案**

## 疑惑交流

## 任务清单
- //预审已完成
- [x] 11.07(Sat), 参加预审，目标A档前两名
- [x] 11.06(Fri), 测试通信逻辑是否可用，完成预审要求
- [x] 11.06(Fri), 完善小车运行和道具寻找、道具使用逻辑
- [x] 11.05(Thur), 测试红外功能
- [x] 11.02(Mon), 测试Vpropi是否好用
- [x] 11.01(Sun), 通信logic完善接收（传输校验），发送
- [x] 11.01(Sun), 调出红外循迹
- [x] 10.30(Fri)，测试18650电池是否好用
- [x] 10.30(Fri)，分发DISCOVERY板子，入新的传感器
- [x] 10.30(Fri)，焊接两块板子，测试电路板是否可以使用

## 预审方案
1. 使用伯文画的大板子
2. 红外循迹，暂时不考虑获得地图数据并储存
3. 小车的速度尽可能快
4. 实现防御功能
