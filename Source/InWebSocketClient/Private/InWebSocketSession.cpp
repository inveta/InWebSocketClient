// Fill out your copyright notice in the Description page of Project Settings.


#include "InWebSocketSession.h"
#include "IWebSocket.h"
#include "WebSocketsModule.h"

UInWebSocketSession* UInWebSocketSession::CreateWebSocket(const FString& Url)
{
	UInWebSocketSession* ptr =  NewObject<UInWebSocketSession>();
	ptr->m_Url = Url;
	return ptr;
}

void UInWebSocketSession::Activate()
{
	UE_LOG(LogInWebSocketClientModule, Log, TEXT("UInWebSocketSession Activate url=%s"), *m_Url);
	Connect();
}

void UInWebSocketSession::Connect()
{
	if (nullptr != m_NativeSocket)
	{
		UE_LOG(LogInWebSocketClientModule, Warning,TEXT("UInWebSocketSession nullptr != m_NativeSocket "));
		return;
	}
	m_NativeSocket = FWebSocketsModule::Get().CreateWebSocket(m_Url);
	m_NativeSocket->OnConnected().AddUObject(this, &UInWebSocketSession::OnConnected);
	m_NativeSocket->OnClosed().AddUObject(this, &UInWebSocketSession::OnClosed);
	m_NativeSocket->OnMessage().AddUObject(this, &UInWebSocketSession::OnMessage);
	m_NativeSocket->OnRawMessage().AddUObject(this, &UInWebSocketSession::OnRawMessage);
	m_NativeSocket->OnConnectionError().AddUObject(this, &UInWebSocketSession::OnError);

	m_NativeSocket->Connect();
}
void UInWebSocketSession::Close()
{
	if (false == IsConnected())
	{
		m_NativeSocket = nullptr;
		SetReadyToDestroy();
		return;
	}
	m_NativeSocket->Close();
	m_NativeSocket = nullptr;
	SetReadyToDestroy();
}

void UInWebSocketSession::SendMessage(const FString& Message)
{
	if (false == IsConnected())
	{
		UE_LOG(LogInWebSocketClientModule, Error, TEXT("UInWebSocketSession IsConnected false"));
		return;
	}
	if (Message.Len() <= 0)
	{
		UE_LOG(LogInWebSocketClientModule, Error, TEXT("UInWebSocketSession SendMessage Message is empty"));
		return;
	}
	UE_LOG(LogInWebSocketClientModule, Log, TEXT("UInWebSocketSession SendMessage Message=%s"), *Message);
	m_NativeSocket->Send(Message);
}
bool UInWebSocketSession::IsConnected()
{
	if (nullptr == m_NativeSocket)
	{
		return false;
	}
	return m_NativeSocket->IsConnected();

}
void UInWebSocketSession::OnConnected()
{
	UE_LOG(LogInWebSocketClientModule, Log, TEXT("UInWebSocketSession OnConnected Success url=%s"),*m_Url);

	OnConnectedEvent.Broadcast(this,"");
}
void UInWebSocketSession::OnError(const FString& Error)
{
	UE_LOG(LogInWebSocketClientModule, Error, TEXT("UInWebSocketSession OnError url=%s error=%s"), *m_Url, *Error);
	OnClosedEvent.Broadcast(this, "");
}
void UInWebSocketSession::OnClosed(int32 StatusCode, const FString& Reason, bool bWasClean)
{
	UE_LOG(LogInWebSocketClientModule, Log, TEXT("UInWebSocketSession OnClosed  url=%s StatusCode=%d Reason=%s bWasClean=%d"),
		*m_Url, StatusCode, *Reason, bWasClean);

	OnClosedEvent.Broadcast(this,"");
}
void UInWebSocketSession::OnMessage(const FString& MessageString)
{
	UE_LOG(LogInWebSocketClientModule, Log, TEXT("UInWebSocketSession OnMessage MessageString=%s"), *MessageString);

	OnMessageEvent.Broadcast(this,MessageString);
}
void UInWebSocketSession::OnRawMessage(const void* Data, SIZE_T  Size, SIZE_T  BytesRemaining)
{

}