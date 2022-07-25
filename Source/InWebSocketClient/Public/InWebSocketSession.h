// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "InWebSocketSession.generated.h"

class IWebSocket;

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class INWEBSOCKETCLIENT_API UInWebSocketSession : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = InWebSocket)
		static  UInWebSocketSession* CreateWebSocket(const FString& Url);

	UFUNCTION(BlueprintCallable, Category = InWebSocket)
		void Close();

	UFUNCTION(BlueprintCallable, Category = InWebSocket)
		void SendMessage(const FString& Message);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FInWebSocketEvent, const UInWebSocketSession*, ws , const FString&, MessageString);
	UPROPERTY(EditAnywhere, BlueprintReadOnly, BlueprintAssignable, Category = InWebSocket)
	FInWebSocketEvent OnMessageEvent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, BlueprintAssignable, Category = InWebSocket)
	FInWebSocketEvent OnConnectedEvent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, BlueprintAssignable, Category = InWebSocket)
	FInWebSocketEvent OnClosedEvent;


	virtual void Activate();
	void Connect();
private:
	bool IsConnected();
	void OnConnected();
	void OnClosed(int32 StatusCode, const FString& Reason, bool bWasClean);
	void OnMessage(const FString&  MessageString );
	void OnRawMessage(const void*  Data , SIZE_T  Size , SIZE_T  BytesRemaining );
	void OnError(const FString& Error);
public:
	FString m_Url;
private:
	TSharedPtr<IWebSocket> m_NativeSocket = nullptr;
	TMap<FString, FString> m_Headers;
	
};
