void drv_TimeSysTickHandler (void)
{
	if (msCount > 0) msCount--;

	if (g1msTimerUART1 > 0) g1msTimerUART1--;

	if (gTcpTimeout > 0) gTcpTimeout--;

	if (gHeartbeatTimeout > 0) gHeartbeatTimeout--;

	if(g1msTimerUART2>0)g1msTimerUART2--;
	
	if(g1msTimerUART4>0)g1msTimerUART4--;
}

