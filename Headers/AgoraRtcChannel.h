//
//  AgoraRtcChannel.h
//  AgoraRtcChannel
//
//  Copyright (c) 2019 Agora. All rights reserved.
//

#import "AgoraRtcEngineKit.h"

@class AgoraRtcChannelMediaOptions;
@protocol AgoraRtcChannelDelegate;
@interface AgoraRtcEngineKit ()


/** Creates and gets an AgoraRtcChannel instance.

To join more than one channel, call this method multiple times to create as many AgoraRtcChannel objects as needed, and call the [joinChannelByToken]([AgoraRtcChannel joinChannelByToken:info:uid:options:]) method of each created AgoraRtcChannel instance.

After joining multiple channels, you can simultaneously subscribe to streams of all the channels, but publish a stream in only one channel at one time.

@param channelId The unique channel name for an Agora RTC session. It must be in the string format and not exceed 64 bytes in length. Supported character scopes are:

- All lowercase English letters: a to z.
- All uppercase English letters: A to Z.
- All numeric characters: 0 to 9.
- The space character.
- Punctuation characters and other symbols, including: "!", "#", "$", "%", "&", "(", ")", "+", "-", ":", ";", "<", "=", ".", ">", "?", "@", "[", "]", "^", "_", " {", "}", "|", "~", ",".

**Note**

- This parameter does not have a default value. You must set it.
- Do not set it as the empty string "". Otherwise, the SDK returns `AgoraErrorCodeRefused`(-5).

@return - A pointer to the AgoraRtcChannel instance, if the method call succeeds.
- An empty pointer `nil`, if the method call fails.
- `AgoraErrorCodeRefused`(-5), if you set `channelId` as the empty string "".
 */
- (AgoraRtcChannel * _Nullable)createRtcChannel:(NSString * _Nonnull)channelId;

@end

/** The AgoraRtcChannel class.

 **Since** v3.0.0.
 */
__attribute__((visibility("default"))) @interface AgoraRtcChannel : NSObject

/** Destroys the AgoraRtcChannel instance.

 @return - 0: Success.
 - < 0: Failure.

   - `AgoraErrorCodeNotInitialized`(-7): The AgoraRtcChannel instance is not initialized before calling this method.
 */
- (int)destroy;
/** Sets the channel delegate.

 After setting the channel delegate, you can listen for channel events and receive the statistics of the corresponding AgoraRtcChannel instance.

 @param channelDelegate The delegate of the AgoraRtcChannel instance. For details, see AgoraRtcChannelDelegate.
 */
- (void)setRtcChannelDelegate:(id<AgoraRtcChannelDelegate> _Nullable)channelDelegate;

/** Gets the channel ID of the current AgoraRtcChannel instance.

 @return - The channel ID of the current AgoraRtcChannel instance, if the method call succeeds.
 - The empty string "", if the method call fails.
 */
- (NSString * _Nullable)getChannelId;

/** Gets the current call ID.

 @return - The current call ID, if the method call succeeds.
 - The empty string "", if the method call fails.
 */
- (NSString * _Nullable)getCallId;
/** Gets the connection state of the SDK.

 @return The connection state, see [AgoraConnectionStateType](AgoraConnectionStateType):

  - `AgoraConnectionStateDisconnected`(1): The SDK is disconnected from Agora's edge server.
  - `AgoraConnectionStateConnecting`(2): The SDK is connecting to Agora's edge server.
  - `AgoraConnectionStateConnected`(3): The SDK joined a channel and is connected to Agora's edge server. You can now publish or subscribe to a media stream in the channel.
  - `AgoraConnectionStateReconnecting`(4): The SDK keeps rejoining the channel after being disconnected from a joined channel because of network issues.
  - `AgoraConnectionStateFailed`(5): The SDK fails to join the channel.
 */
- (AgoraConnectionStateType)getConnectionState;

/** Joins the channel with a user ID.

 This method differs from the [joinChannelByToken]([AgoraRtcEngineKit joinChannelByToken:channelId:info:uid:joinSuccess:]) method in the AgoraRtcEngineKit class in the following aspects:

| [AgoraRtcChannel joinChannelByToken]([AgoraRtcChannel joinChannelByToken:info:uid:options:])                                                  | [AgoraRtcEngineKit joinChannelByToken]([AgoraRtcEngineKit joinChannelByToken:channelId:info:uid:joinSuccess:])                                                                                       |
|------------------------------------------------------------------------------------------------------------------------------------------------|--------------------------------------------------------------------------------------------------------------|
| Does not contain the `channelId` parameter, because `channelId` is specified when creating the AgoraRtcChannel instance.                                | Contains the `channelId` parameter, which specifies the channel to join.                                       |
| Contains the `options` parameter, which decides whether to subscribe to all streams before joining the channel.                                  | Does not contain the `options` parameter. By default, users subscribe to all streams when joining the channel. |
| Users can join multiple channels simultaneously by creating multiple AgoraRtcChannel instances and calling the `joinChannelByToken` method of each instance. | Users can join only one channel.                                                                             |
| By default, the SDK does not publish any stream after the user joins the channel. You need to call the [publish]([AgoraRtcChannel publish]) method to do that.              | By default, the SDK publishes streams once the user joins the channel.                                       |

 **Note**

 - If you are already in a channel, you cannot rejoin it with the same `uid`.
 - We recommend using different `uid` for different channels.
 - If you want to join the same channel from different devices, ensure that the `uid` in all devices are different.
 - Ensure that the app ID you use to generate the token is the same with the app ID used when creating the AgoraRtcEngineKit instance.

 @param token The token for authentication:

 - In situations not requiring high security: You can use the temporary token generated at Agora Console. For details, see [Get a temporary token](https://docs.agora.io/en/Agora%20Platform/token?platform=All%20Platforms#get-a-temporary-token).
 - In situations requiring high security: Set it as the token generated at your server. For details, see [Generate a token](https://docs.agora.io/en/Interactive%20Broadcast/token_server_cpp?platform=CPP).
 @param info Additional information about the channel. This parameter can be set as null. Other users in the channel do not receive this information.
 @param uid The user ID. A 32-bit unsigned integer with a value ranging from 1 to (2<sup>32</sup>-1). This parameter must be unique. If `uid` is not assigned (or set as 0), the SDK assigns a `uid` and reports it in the [rtcChannelDidJoinChannel]([AgoraRtcChannelDelegate rtcChannelDidJoinChannel:withUid:elapsed:]) callback. The app must maintain this user ID.
 @param options The channel media options: AgoraRtcChannelMediaOptions

 @return - `0`(`AgoraRtmpStreamingErrorCodeOK`): Success.
- < `0`: Failure.

  - `-2`(`AgoraErrorCodeInvalidArgument`): The parameter is invalid.
  - `-3`(`AgoraErrorCodeNotReady`): The SDK fails to be initialized. You can try re-initializing the SDK.
  - `-5`(`AgoraErrorCodeRefused`): The request is rejected. This may be caused by the following:
    
    - You have created an `AgoraRtcChannel` object with the same channel name.
    - You have joined and published a stream in a channel created by the `AgoraRtcChannel` object.
 */
- (int)joinChannelByToken:(NSString * _Nullable)token
                       info:(NSString * _Nullable)info
                        uid:(NSUInteger)uid
                    options:(AgoraRtcChannelMediaOptions * _Nonnull)options;

/** Joins a channel with the user account.

 This method differs from the [joinChannelByUserAccount]([AgoraRtcEngineKit joinChannelByUserAccount:token:channelId:joinSuccess:]) method in the AgoraRtcEngineKit class in the following aspects:

| [AgoraRtcChannel joinChannelByUserAccount]([AgoraRtcChannel joinChannelByUserAccount:token:options:])                                                  | [AgoraRtcEngineKit joinChannelByUserAccount]([AgoraRtcEngineKit joinChannelByUserAccount:token:channelId:joinSuccess:])                                                                                      |
|------------------------------------------------------------------------------------------------------------------------------------------------|--------------------------------------------------------------------------------------------------------------|
| Does not contain the `channelId` parameter, because `channelId` is specified when creating the AgoraRtcChannel instance.                                | Contains the `channelId` parameter, which specifies the channel to join.                                       |
| Contains the `options` parameter, which decides whether to subscribe to all streams before joining the channel.                                  | Does not contain the `options` parameter. By default, users subscribe to all streams when joining the channel. |
| Users can join multiple channels simultaneously by creating multiple AgoraRtcChannel instances and calling the `joinChannelByUserAccount` method of each instance. | Users can join only one channel.                                                                             |
| By default, the SDK does not publish any stream after the user joins the channel. You need to call the [publish]([AgoraRtcChannel publish]) method to do that.              | By default, the SDK publishes streams once the user joins the channel.                                       |

 **Note**

 - If you are already in a channel, you cannot rejoin it with the same `uid`.
 - We recommend using different `uid` for different channels.
 - If you want to join the same channel from different devices, ensure that the `uid` in all devices are different.
 - Ensure that the app ID you use to generate the token is the same with the app ID used when creating the AgoraRtcEngineKit instance.

 @param userAccount The user account. The maximum length of this parameter is 255 bytes. Ensure that you set this parameter and do not set it as null.

   - All lowercase English letters: a to z.
   - All uppercase English letters: A to Z.
   - All numeric characters: 0 to 9.
   - The space character.
   - Punctuation characters and other symbols, including: "!", "#", "$", "%", "&", "(", ")", "+", "-", ":", ";", "<", "=", ".", ">", "?", "@", "[", "]", "^", "_", " {", "}", "|", "~", ",".
 @param token The token generated at your server:

 - In situations not requiring high security: You can use the temporary token generated at Console. For details, see [Get a temporary token](https://docs.agora.io/en/Agora%20Platform/token?platform=All%20Platforms#get-a-temporary-token).
 - In situations requiring high security: Set it as the token generated at your server. For details, see [Get a token](https://docs.agora.io/en/Agora%20Platform/token?platform=All%20Platforms&_ga=2.220734490.38407571.1574824837-569329450.1562664463#get-a-token).
 @param options The channel media options: AgoraRtcChannelMediaOptions

 @return - 0: Success.
 - < 0: Failure.

   - `AgoraErrorCodeInvalidArgument`(-2)
   - `AgoraErrorCodeNotReady`(-3)
   - `AgoraErrorCodeRefused`(-5)
 */
- (int)joinChannelByUserAccount:(NSString * _Nonnull)userAccount
                            token:(NSString * _Nullable)token
                          options:(AgoraRtcChannelMediaOptions * _Nonnull)options;

/** Leaves the current channel.

 A successful `leaveChannel` method call triggers the following callbacks:

  - The local client: [rtcChannelDidLeaveChannel]([AgoraRtcChannelDelegate rtcChannelDidLeaveChannel:withStats:]).
  - The remote client: [didOfflineOfUid]([AgoraRtcChannelDelegate rtcChannel:didOfflineOfUid:reason:]), if the user leaving the channel is in a Communication channel, or is a host in a Live-Broadcast channel.

 @return - `0`(`AgoraRtmpStreamingErrorCodeOK`): Success.
- < `0`: Failure.

  - `-1`(`AgoraErrorCodeFailed`): A general error occurs (no specified reason).
  - `-2`(`AgoraErrorCodeInvalidArgument`): The parameter is invalid.
  - `-7`(`AgoraErrorCodeNotInitialized`): The SDK is not initialized.
 */
- (int)leaveChannel;

/** Publishes the local stream to the channel.

 You must keep the following restrictions in mind when calling this method. Otherwise, the SDK returns the AgoraErrorCodeRefused(-5)：

 - This method publishes one stream only to the channel corresponding to the current AgoraRtcChannel instance.
 - In a Live-Broadcast channel, only a host can call this method. To switch the client role, call [setClientRole]([AgoraRtcChannel setClientRole:]) of the current AgoraRtcChannel instance.
 - You can publish a stream to only one channel at a time. For details, see the advanced guide *Join Multiple Channels*.

 @return - 0: Success.
 - < 0: Failure.

   - `AgoraErrorCodeRefused`(-5): The method call is refused.
 */
- (int)publish;

/** Stops publishing a stream to the channel.

 If you call this method in a channel where you are not publishing streams, the SDK returns AgoraErrorCodeRefused(-5).

 @return - 0: Success.
 - < 0: Failure.

   - `AgoraErrorCodeRefused`(-5): The method call is refused.
 */
- (int)unpublish;

/** Sets the role of a user.

 This method sets the role of a user, such as a host or an audience. In an interactive live streaming channel, only a host can call the [publish]([AgoraRtcChannel publish]) method in the AgoraRtcChannel class.
 
 A successful call of this method triggers the following callbacks:

 - The local client: [didClientRoleChanged]([AgoraRtcChannelDelegate rtcChannel:didClientRoleChanged:newRole:]).
 - The remote client: [didJoinedOfUid]([AgoraRtcChannelDelegate rtcChannel:didJoinedOfUid:elapsed:]) or [didOfflineOfUid]([AgoraRtcChannelDelegate rtcChannel:didOfflineOfUid:reason:])(AgoraUserOfflineReasonBecomeAudience).

 @param role The role of the user:

 - `AgoraClientRoleBroadcaster(1)`: Host. A host can both send and receive streams.
 - `AgoraClientRoleAudience(2)`: Audience, the default role. An audience can only receive streams.

 @return - `0`(`AgoraRtmpStreamingErrorCodeOK`): Success.
- < `0`: Failure.

  - `-1`(`AgoraErrorCodeFailed`): A general error occurs (no specified reason).
  - `-2`(`AgoraErrorCodeInvalidArgument`): The parameter is invalid.
  - `-7`(`AgoraErrorCodeNotInitialized`): The SDK is not initialized.
 */
- (int)setClientRole:(AgoraClientRole)role;

/** Renews the token when the current token expires.

 In the following situations, the SDK decides that the current token has expired:

 - The SDK triggers the [tokenPrivilegeWillExpire]([AgoraRtcChannelDelegate rtcChannel:tokenPrivilegeWillExpire:]) callback, or
 - The [connectionChangedToState]([AgoraRtcChannelDelegate rtcChannel:connectionChangedToState:reason:]) callback reports the AgoraConnectionChangedTokenExpired(9) error.

 You should get a new token from your server and call this method to renew it. Failure to do so results in the SDK disconnecting from the Agora server.

 **Note**

 Agora recommends using the [rtcChannelRequestToken]([AgoraRtcChannelDelegate rtcChannelRequestToken:]) callback to report the AgoraErrorCodeTokenExpired(-109) error, not using the [didOccurError]([AgoraRtcChannelDelegate rtcChannel:didOccurError:]) callback.

 @param token The new token.

 @return - `0`(`AgoraRtmpStreamingErrorCodeOK`): Success.
- < `0`: Failure.

  - `-1`(`AgoraErrorCodeFailed`): A general error occurs (no specified reason).
  - `-2`(`AgoraErrorCodeInvalidArgument`): The parameter is invalid.
  - `-7`(`AgoraErrorCodeNotInitialized`): The SDK is not initialized.
 */
- (int)renewToken:(NSString * _Nonnull)token;

/** Enables built-in encryption with an encryption password before joining a channel.
 
 **DEPRECATED** from v3.1.0. Use [enableEncryption]([AgoraRtcEngineKit enableEncryption:encryptionConfig:]) instead.

 All users in a channel must set the same encryption password. The encryption password is automatically cleared once a user leaves the channel.

 If the encryption password is not specified or set to empty, the encryption functionality is disabled.

 **Note**

 - Do not use this method for CDN live streaming.
 - For optimal transmission, ensure that the encrypted data size does not exceed the original data size + 16 bytes. 16 bytes is the maximum padding size for AES encryption.

 @param secret Encryption password.

 @return - 0: Success.
 - < 0: Failure.
 */
- (int)setEncryptionSecret:(NSString * _Nullable)secret;

/** Sets the built-in encryption mode.
 
 **DEPRECATED** from v3.1.0. Use [enableEncryption]([AgoraRtcEngineKit enableEncryption:encryptionConfig:]) instead.

 The SDK supports built-in encryption, which is set to the "aes-128-xts" mode by default. Call this method to use other encryption modes.

 All users in the same channel must use the same encryption mode and password.

 Refer to the information related to the AES encryption algorithm on the differences between the encryption modes.

 **Note**

 - Call the [setEncryptionSecret]([AgoraRtcChannel setEncryptionSecret:]) method to enable the built-in encryption function before calling this method.
 - Do not use this method for CDN live streaming.

 @param encryptionMode Sets the encryption mode.

 - “aes-128-xts”: (default) 128-bit AES encryption, XTS mode.
 - “aes-256-xts”: 256-bit AES encryption, XTS mode.
 - “aes-128-ecb”: 128-bit AES encryption, ECB mode.
 - "": (Default) When encryptionMode is set as the empty string, the encryption is in “aes-128-xts” by default.

 @return - 0: Success.
 - < 0: Failure.
 */
- (int)setEncryptionMode:(NSString * _Nullable)encryptionMode;
/** Enables/Disables the built-in encryption.

 In scenarios requiring high security, Agora recommends calling `enableEncryption` to enable the built-in encryption before joining a channel.
 
 All users in the same channel must use the same encryption mode and encryption key. Once all users leave the channel, the encryption key of this channel is automatically cleared.
 
 **Note**

 - If you enable the built-in encryption, you cannot use the RTMP streaming function.
 - Agora supports four encryption modes. If you choose an encryption mode (excepting `SM4_128_ECB` mode), you need to add an external encryption library when integrating the SDK. See *Channel Encryption*.

 @param enabled Whether to enable the built-in encryption:
 - YES: Enable the built-in encryption.
 - NO: Disable the built-in encryption.

 @param config Configurations of built-in encryption schemas. See AgoraEncryptionConfig.

 @return - 0: Success.
 - < 0: Failure.

  - -2 (`AgoraErrorCodeInvalidArgument`): An invalid parameter is used. Set the parameter with a valid value.
  - -7 (`AgoraErrorCodeNotInitialized`): The SDK is not initialized. Initialize the `AgoraRtcEngineKit` instance before calling this method.
  - -4 (`AgoraErrorCodeNotSupported`): The encryption mode is incorrect or the SDK fails to load the external encryption library. Check the enumeration or reload the external encryption library.
 */
- (int)enableEncryption:(bool)enabled encryptionConfig:(AgoraEncryptionConfig *)config;
/** Sets the priority of a remote user’s stream.

 Use this method with the [setRemoteSubscribeFallbackOption]([AgoraRtcEngineKit setRemoteSubscribeFallbackOption:]) method. If the fallback function is enabled for a remote stream, the SDK ensures the high-priority user gets the best possible stream quality.

 **Note**

 The SDK supports setting userPriority as high for one user only.

 @param uid The ID of the remote user.
 @param userPriority The priority of the remote user, see AgoraUserPriority.

 @return - 0: Success.
 - < 0: Failure.
 */
- (int)setRemoteUserPriority:(NSUInteger)uid
                        type:(AgoraUserPriority)userPriority;
/** Sets the sound position and gain of a remote user.

 When the local user calls this method to set the sound position of a remote user, the sound difference between the left and right channels allows the local user to track the real-time position of the remote user, creating a real sense of space. This method applies to massively multiplayer online games, such as Battle Royale games.

 **Note**

 - For this method to work, enable stereo panning for remote users by calling [enableSoundPositionIndication]([AgoraRtcEngineKit enableSoundPositionIndication:]) before joining a channel. This method requires hardware support.
 - For the best effect, we recommend using the following audio output devices:

   - (iOS) A stereo headset.
   - (macOS) A stereo loudspeaker.

 @param uid The ID of the remote user.
 @param pan The sound position of the remote user. The value ranges from -1.0 to 1.0:

 - 0.0: (Default) The remote sound comes from the front.
 - -1.0: The remote sound comes from the left.
 - 1.0: The remote sound comes from the right.
 @param gain Gain of the remote user. The value ranges from 0.0 to 100.0. The default value is 100.0 (the original gain of the remote user). The smaller the value, the less the gain.

 @return - 0: Success.
 - < 0: Failure.
 */
- (int) setRemoteVoicePosition:(NSUInteger) uid
                           pan:(double) pan
                          gain:(double) gain;
/** Updates the display mode of the video view of a remote user.

 After initializing the video view of a remote user, you can call this method to update its rendering and mirror modes. This method affects only the video view that the local user sees.

 **Note**

 - Ensure that you have called [setupRemoteVideo]([AgoraRtcEngineKit setupRemoteVideo:]) to initialize the remote video view before calling this method.
 - During a call, you can call this method as many times as necessary to update the display mode of the video view of a remote user.

 @param uid The ID of the remote user.
 @param renderMode The rendering mode of the remote video view. See [AgoraVideoRenderMode](AgoraVideoRenderMode).
 @param mirrorMode The mirror mode of the remote video view. See [AgoraVideoMirrorMode](AgoraVideoMirrorMode).

 **Note**

 The SDK disables the mirror mode by default.

 @return - 0: Success.
 - < 0: Failure.
 */
- (int)setRemoteRenderMode:(NSUInteger)uid
                renderMode:(AgoraVideoRenderMode)renderMode
                mirrorMode:(AgoraVideoMirrorMode)mirrorMode;

/** Sets whether to receive all remote audio streams by default.

 You can call this method either before or after joining a channel. If you call `setDefaultMuteAllRemoteAudioStreams (YES)` after joining a channel, the remote audio streams of all subsequent users are not received.

 @note If you want to resume receiving the audio stream, call [muteRemoteAudioStream (NO)]([AgoraRtcChannel muteRemoteAudioStream:mute:]), and specify the ID of the remote user whose audio stream you want to receive. To receive the audio streams of multiple remote users, call `muteRemoteAudioStream (NO)` as many times. Calling `setDefaultMuteAllRemoteAudioStreams (NO)` resumes receiving the audio streams of subsequent users only.

 @param mute Sets whether to receive/stop receiving all remote audio streams by default:

 - YES: Stop receiving all remote audio streams by default.
 - NO: (Default) Receive all remote audio streams by default.

 @return - 0: Success.
 - < 0: Failure.
 */
- (int)setDefaultMuteAllRemoteAudioStreams:(BOOL)mute;

/** Sets whether to receive all remote video streams by default.

 You can call this method either before or after joining a channel. If you call `setDefaultMuteAllRemoteVideoStreams (YES)` after joining a channel, the remote video streams of all subsequent users are not received.

 @note If you want to resume receiving the video stream, call [muteRemoteVideoStream (NO)]([AgoraRtcEngineKit muteRemoteVideoStream:mute:]), and specify the ID of the remote user whose video stream you want to receive. To receive the video streams of multiple remote users, call `muteRemoteVideoStream (NO)` as many times. Calling `setDefaultMuteAllRemoteVideoStreams (NO)` resumes receiving the video streams of subsequent users only.

 @param mute Sets whether to receive/stop receiving all remote video streams by default.

 - YES: Stop receiving all remote video streams by default.
 - NO: (Default) Receive all remote video streams by default.

 @return - 0: Success.
 - < 0: Failure.
 */
- (int)setDefaultMuteAllRemoteVideoStreams:(BOOL)mute;

/** Receives/Stops receiving a specified remote user’s audio stream.

 **Note**

 If you call the [muteAllRemoteAudioStreams]([AgoraRtcChannel muteAllRemoteAudioStreams:]) method and set `mute` as `YES` to mute all remote audio streams, call the `muteAllRemoteAudioStreams` method again and set `mute` as `NO` before calling this method. The `muteAllRemoteAudioStreams` method sets all remote streams, while the `muteRemoteAudioStream` method sets a specified stream.

 @param uid User ID of the specified remote user.
 @param mute Sets whether to receive/stop receiving a specified remote user’s audio stream:

 - YES: Stop receiving a specified remote user’s audio stream.
 - NO: (Default) Receive a specified remote user’s audio stream.

 @return - 0: Success.
 - < 0: Failure.
 */
- (int)muteRemoteAudioStream:(NSUInteger)uid
                        mute:(BOOL)mute;
/** Adjust the playback volume of a specified remote user.

 You can call this method as many times as necessary to adjust the playback volume of different remote users, or to repeatedly adjust the playback volume of the same remote user.

 **Note**

 - Call this method after joining a channel.
 - The playback volume here refers to the mixed volume of a specified remote user.
 - This method can only adjust the playback volume of one specified remote user at a time. To adjust the playback volume of different remote users, call the method as many times, once for each remote user.

 @param uid The ID of the remote user.
 @param volume The playback volume of the specified remote user. The value ranges from 0 to 100:

 - 0: Mute.
 - 100: Original volume.

 @return - 0: Success.
 - < 0: Failure.
 */
- (int)adjustUserPlaybackSignalVolume:(NSUInteger)uid
                               volume:(int)volume;

/** Receives/Stops receiving all remote audio streams.

 @param mute Sets whether to receive/stop receiving all remote audio streams:

 - YES: Stop receiving all remote audio streams.
 - NO: (Default) Receive all remote audio streams.

 @return - 0: Success.
 - < 0: Failure.
 */
- (int)muteAllRemoteAudioStreams:(BOOL)mute;

/** Receives/Stops receiving a specified remote user’s video stream.

 **Note**

 If you call the [muteAllRemoteVideoStreams]([AgoraRtcChannel muteAllRemoteVideoStreams:]) method and set `mute` as `YES` to stop receiving all remote video streams, call the `muteAllRemoteVideoStreams` method again and set `mute` as `NO` before calling this method.

 @param uid User ID of the specified remote user.
 @param mute Sets whether to receive/stop receiving a specified remote user’s video stream.

 - YES: Stops receiving a specified remote user’s video stream.
 - NO: (Default) Receives a specified remote user’s video stream.

 @return - 0: Success.
 - < 0: Failure.
 */
- (int)muteRemoteVideoStream:(NSUInteger)uid
                        mute:(BOOL)mute;

/** Receives/Stops receiving all remote video streams.

 @param mute Sets whether to receive/stop receiving all remote video streams:

 - YES: Stops receiving all remote video streams.
 - NO: (Default) Receives all remote video streams.

 @return - 0: Success.
 - < 0: Failure.
 */
- (int)muteAllRemoteVideoStreams:(BOOL)mute;
/** Sets the stream type of the remote video.

 Under limited network conditions, if the publisher has not disabled the dual-stream mode using [enableDualStreamMode(NO)]([AgoraRtcEngineKit enableDualStreamMode:])enableDualStreamMode(NO), the receiver can choose to receive either the high-quality video stream (the high resolution, and high bitrate video stream) or the low-quality video stream (the low resolution, and low bitrate video stream).

 By default, users receive the high-quality video stream. Call this method if you want to switch to the low-quality video stream. This method allows the app to adjust the corresponding video stream type based on the size of the video window to reduce the bandwidth and resources.

 The aspect ratio of the low-quality video stream is the same as the high-quality video stream. Once the resolution of the high-quality video stream is set, the system automatically sets the resolution, frame rate, and bitrate of the low-quality video stream.

 The SDK reports the result of calling this method in the [didApiCallExecute]([AgoraRtcEngineDelegate rtcEngine:didApiCallExecute:api:result:]) callback.

 @param uid ID of the remote user sending the video stream.
 @param streamType Sets the video-stream type. See AgoraVideoStreamType.

 @return * 0: Success.
* < 0: Failure.
 */
- (int)setRemoteVideoStream:(NSUInteger)uid
                       type:(AgoraVideoStreamType)streamType;
/** Sets the default video-stream type for the video received by the local user when the remote user sends dual streams.

 @param streamType Sets the default video-stream type. See AgoraVideoStreamType.

 @return - 0: Success.
 - < 0: Failure.
 */
- (int)setRemoteDefaultVideoStreamType:(AgoraVideoStreamType)streamType;
/** Adds a voice or video stream RTMP URL address to the live interactive streaming.

 If this method call is successful, the server pulls the voice or video stream and injects it into a live channel. This is applicable to scenarios where all audience members in the channel can watch a live show and interact with each other.

 The `addInjectStreamUrl` method call triggers the following callbacks:

 - The local client:

   - [streamInjectedStatusOfUrl]([AgoraRtcChannelDelegate rtcChannel:streamInjectedStatusOfUrl:uid:status:]), with the state of the injecting the online stream.
   - [didJoinedOfUid]([AgoraRtcChannelDelegate rtcChannel:didJoinedOfUid:elapsed:])(uid: 666), if the method call is successful and the online media stream is injected into the channel.

 - The remote client:

   - [didJoinedOfUid]([AgoraRtcChannelDelegate rtcChannel:didJoinedOfUid:elapsed:])(uid: 666), if the method call is successful and the online media stream is injected into the channel.

 **Note:**

 - You can inject only one media stream into the channel at the same time.
 - Ensure that you enable the RTMP Converter service before using this function. See [Prerequisites](https://docs.agora.io/en/Interactive%20Broadcast/cdn_streaming_apple?platform=iOS#prerequisites).
 - This method applies to the Native SDK v2.4.1 and later.

 @param url    URL address to be added to the ongoing live interactive streaming. Valid protocols are RTMP, HLS, and HTTP-FLV.

- Supported audio codec type: AAC.
- Supported video codec type: H264 (AVC).
 @param config AgoraLiveInjectStreamConfig object which contains the configuration information for the added voice or video stream.

@return * 0: Success.
* < 0: Failure.

    - `AgoraErrorCodeInvalidArgument`(-2): The injected URL does not exist. Call this method again to inject the stream and ensure that the URL is valid.
    - `AgoraErrorCodeNotReady`(-3): The user is not in the channel.
    - `AgoraErrorCodeNotSupported`(-4): The channel profile is not Live Broadcast.
    - `AgoraErrorCodeNotInitialized`(-7): The SDK is not initialized. Ensure that the AgoraRtcChannel object is initialized before using this method.
 */
- (int)addInjectStreamUrl:(NSString * _Nonnull)url config:(AgoraLiveInjectStreamConfig * _Nonnull)config;
/** Removes the voice or video stream RTMP URL address from the live interactive streaming.

 This method removes the URL address (added by the [addInjectStreamUrl]([AgoraRtcChannel addInjectStreamUrl:config:]) method) from the live interactive streaming.

 If this method call is successful, the SDK triggers the [didOfflineOfUid]([AgoraRtcChannelDelegate rtcChannel:didOfflineOfUid:reason:]) callback and returns a stream uid of 666.

 @param url URL address of the added stream to be removed.

 @return * 0: Success.
 * < 0: Failure.
 */
- (int)removeInjectStreamUrl:(NSString * _Nonnull)url;
/** Publishes the local stream to the CDN.

 This method call triggers the [rtmpStreamingChangedToState]([AgoraRtcChannelDelegate rtcChannel:rtmpStreamingChangedToState:state:errorCode:]) callback on the local client to report the state of adding a local stream to the CDN.

 **Note:**

 - This method applies to the live interactive streaming profile only.
 - Ensure that the user joins the channel before calling this method.
 - Ensure that you enable the RTMP Converter service before using this function. See [Prerequisites](https://docs.agora.io/en/Interactive%20Broadcast/cdn_streaming_apple?platform=iOS#prerequisites).
 - This method adds only one stream URL each time it is called.

 @param url  The CDN streaming URL in the RTMP format. The maximum length of this parameter is 1024 bytes. The RTMP URL address must not contain special characters, such as Chinese language characters.
 @param transcodingEnabled Sets whether transcoding is enabled/disabled:

 - YES: Enable transcoding. To [transcode](https://docs.agora.io/en/Agora%20Platform/terms?platform=All%20Platforms#transcoding) the audio or video streams when publishing them to CDN live, often used for combining the audio and video streams of multiple hosts in CDN live. If you set this parameter as `YES`, ensure that you call the [setLiveTranscoding]([AgoraRtcChannel setLiveTranscoding:]) method before this method.
 - NO: Disable transcoding.

 @return * 0: Success.
 * < 0: Failure.

  - `AgoraErrorCodeInvalidArgument`(-2): Invalid parameter. The URL is nil or the string length is 0.
  - `AgoraErrorCodeNotInitialized`(-7): You have not initialized the RTC Engine when publishing the stream.
 */
- (int)addPublishStreamUrl:(NSString * _Nonnull)url transcodingEnabled:(BOOL)transcodingEnabled;
/** Removes an RTMP stream from the CDN.

This method removes the RTMP URL address added by the [addPublishStreamUrl]([AgoraRtcChannel addPublishStreamUrl:transcodingEnabled:]) method from a CDN live stream.

This method call triggers the [rtmpStreamingChangedToState]([AgoraRtcChannelDelegate rtcChannel:rtmpStreamingChangedToState:state:errorCode:]) callback on the local client to report the state of removing an RTMP stream from the CDN.

 **Note:**

 * This method applies to the live interactive streaming profile only.
 * This method removes only one URL each time it is called.
 * The URL must not contain special characters, such as Chinese language characters.

 @param url The RTMP URL address to be removed. The maximum length of this parameter is 1024 bytes.

 @return * 0: Success.
 * < 0: Failure.
 */
- (int)removePublishStreamUrl:(NSString * _Nonnull)url;
/** Sets the video layout and audio settings for CDN live. (CDN live only.)

 The SDK triggers the [rtcEngineTranscodingUpdated]([AgoraRtcEngineDelegate rtcEngineTranscodingUpdated:]) callback when you call the `setLiveTranscoding` method to update the transcoding setting.

 **Note**

 - This method applies to the live interactive streaming profile only.
 - Ensure that you enable the RTMP Converter service before using this function. See [Prerequisites](https://docs.agora.io/en/Interactive%20Broadcast/cdn_streaming_apple?platform=iOS#prerequisites).
 - If you call the `setLiveTranscoding` method to update the transcoding setting for the first time, the SDK does not trigger the `rtcEngineTranscodingUpdated` callback.

 @param transcoding Sets the CDN live audio/video transcoding settings. See AgoraLiveTranscoding.

 @return * 0: Success.
 * < 0: Failure.
 */
- (int)setLiveTranscoding:(AgoraLiveTranscoding *_Nullable)transcoding;
/** Creates a data stream.

 Each user can create up to five data streams during the lifecycle of the [AgoraRtcChannel](AgoraRtcChannel).

 **Note:**

 Set both the `reliable` and `ordered` parameters to `YES` or `NO`. Do not set one as `YES` and the other as `NO`.

 @param streamId ID of the created data stream.
 @param reliable Sets whether or not the recipients are guaranteed to receive the data stream from the sender within five seconds:

 * YES: The recipients receive the data stream from the sender within five seconds. If the recipient does not receive the data stream within five seconds, an error is reported to the app.
 * NO: There is no guarantee that the recipients receive the data stream within five seconds and no error message is reported for any delay or missing data stream.

 @param ordered  Sets whether or not the recipients receive the data stream in the sent order:

 * YES: The recipients receive the data stream in the sent order.
 * NO: The recipients do not receive the data stream in the sent order.

 @return * 0: Success.
* < 0: Failure.
 */
- (int)createDataStream:(NSInteger * _Nonnull)streamId
               reliable:(BOOL)reliable
                ordered:(BOOL)ordered;
/** Sends data stream messages to all users in a channel.

The SDK has the following restrictions on this method:

* Up to 30 packets can be sent per second in a channel with each packet having a maximum size of 1 kB.
* Each client can send up to 6 kB of data per second.
* Each user can have up to five data streams simultaneously.

A successful `sendStreamMessage` method call triggers the [receiveStreamMessageFromUid]([AgoraRtcChannelDelegate rtcChannel:receiveStreamMessageFromUid:streamId:data:]) callback on the remote client, from which the remote user gets the stream message.

A failed `sendStreamMessage` method call triggers the [didOccurStreamMessageErrorFromUid]([AgoraRtcChannelDelegate rtcChannel:didOccurStreamMessageErrorFromUid:streamId:error:missed:cached:]) callback on the remote client.

 **Note**

 * This method applies only to the Communication profile or to the hosts in the live interactive streaming profile. If an audience in the live interactive streaming profile calls this method, the audience role may be changed to a host.
 * Ensure that you have created the data stream using [createDataStream](createDataStream:reliable:ordered:) before calling this method.

 @param streamId ID of the sent data stream returned in the [createDataStream]([AgoraRtcChannel createDataStream:reliable:ordered:]) method.
 @param data Sent data.

 @return * 0: Success.
 * < 0: Failure.
 */
- (int)sendStreamMessage:(NSInteger)streamId
                    data:(NSData * _Nonnull)data;
/** Starts to relay media streams across channels.

 After a successful method call, the SDK triggers the [channelMediaRelayStateDidChange]([AgoraRtcChannelDelegate rtcChannel:channelMediaRelayStateDidChange:error:]) and [didReceiveChannelMediaRelayEvent]([AgoraRtcChannelDelegate rtcChannel:didReceiveChannelMediaRelayEvent:]) callbacks, and these callbacks return the state and events of the media stream relay.

 - If the `channelMediaRelayStateDidChange` callback returns AgoraChannelMediaRelayStateRunning(2) and AgoraChannelMediaRelayStateIdle(0), and the `didReceiveChannelMediaRelayEvent` callback returns AgoraChannelMediaRelayEventSentToDestinationChannel(4), the SDK starts relaying media streams between the original and the destination channel.
 - If the `channelMediaRelayStateDidChange` callback returns AgoraChannelMediaRelayStateFailure(3), an exception occurs during the media stream relay.

 **Note**

 - Call this method after the [joinChannelByToken]([AgoraRtcChannel joinChannelByToken:info:uid:options:]) method.
 - This method takes effect only when you are a host in the live interactive streaming channel.
 - After a successful method call, if you want to call this method again, ensure that you call the [stopChannelMediaRelay]([AgoraRtcChannel stopChannelMediaRelay]) method to quit the current relay.
 - Contact sales-us@agora.io before implementing this function.
 - We do not support string user accounts in this API.

 @param config The configuration of the media stream relay: [AgoraChannelMediaRelayConfiguration](AgoraChannelMediaRelayConfiguration).

 @return - 0: Success.
 - < 0: Failure.
 */
- (int)startChannelMediaRelay:(AgoraChannelMediaRelayConfiguration * _Nonnull)config;
/** Updates the channels for media stream relay.

 After the channel media relay starts, if you want to relay the media stream to more channels, or leave the current relay channel, you can call the `updateChannelMediaRelay` method.

 After a successful method call, the SDK triggers the [didReceiveChannelMediaRelayEvent]([AgoraRtcChannelDelegate rtcChannel:didReceiveChannelMediaRelayEvent:]) callback with the AgoraChannelMediaRelayEventUpdateDestinationChannel(7) state code.

 **Note**

 - Call this method after the [startChannelMediaRelay]([AgoraRtcChannel startChannelMediaRelay:]) method to update the destination channel.
 - This method supports adding at most four destination channels in the relay. If there are already four destination channels in the relay, remove the unnecessary ones with the `removeDestinationInfoForChannelName` method in channelMediaRelayConfiguration before calling this method.

 @param config The media stream relay configuration: [AgoraChannelMediaRelayConfiguration](AgoraChannelMediaRelayConfiguration).

 @return - 0: Success.
 - < 0: Failure.
 */
- (int)updateChannelMediaRelay:(AgoraChannelMediaRelayConfiguration * _Nonnull)config;
/** Stops the media stream relay.

 Once the relay stops, the host quits all the destination channels.

 After a successful method call, the SDK triggers the [channelMediaRelayStateDidChange]([AgoraRtcChannelDelegate rtcChannel:channelMediaRelayStateDidChange:error:]) callback. If the callback returns AgoraChannelMediaRelayStateIdle(0) and AgoraChannelMediaRelayErrorNone(0), the host successfully stops the relay.

 @note If the method call fails, the SDK triggers the `channelMediaRelayStateDidChange` callback with the AgoraChannelMediaRelayErrorServerNoResponse(2) or AgoraChannelMediaRelayEventUpdateDestinationChannelRefused(8) state code. You can leave the channel by calling the [leaveChannel]([AgoraRtcChannel leaveChannel]) method, and the media stream relay automatically stops.

 @return - 0: Success.
 - < 0: Failure.
 */
- (int)stopChannelMediaRelay;

#if defined (TARGET_OS_IPHONE) && TARGET_OS_IPHONE
- (int)enableRemoteSuperResolution:(NSUInteger)uid enabled:(BOOL)enabled;
#endif

/** Customizes the remote video renderer for the AgoraRtcChannel object.

 During real-time engagements, the Agora SDK enables the default video sink to render the video. In the multi-channel scenario, if you want to customize the video renderer, you can create a customized video sink using AgoraVideoSinkProtocol, and call `setRemoteVideoRenderer` to add the customized sink into the AgoraRtcChannel object.

 @param videoRenderer The customized remote video renderer: AgoraVideoSinkProtocol.
 @param userId The ID of the remote user.
 */
- (void)setRemoteVideoRenderer:(id<AgoraVideoSinkProtocol> _Nullable)videoRenderer forUserId:(NSUInteger)userId;

/** Gets the customized remote video renderer for the AgoraRtcChannel object.

 @param userId The ID of the remote user.

 @return The customized remote video renderer: AgoraVideoSourceProtocol.
*/
- (id<AgoraVideoSinkProtocol> _Nullable)remoteVideoRendererOfUserId:(NSUInteger)userId;

/** Sets the data source of the metadata.

 You need to implement the [AgoraMediaMetadataDataSource](AgoraMediaMetadataDataSource) protocol and specify the type of metadata in this method.

 Use this method with the [setMediaMetadataDelegate]([AgoraRtcChannel setMediaMetadataDelegate:withType:]) method to add synchronized metadata in the video stream. You can create more diversified live interactive streaming interactions, such as sending shopping links, digital coupons, and online quizzes.

 **Note**

 - Call this method before the [joinChannelByToken]([AgoraRtcChannel joinChannelByToken:info:uid:options:]) method.
 - This method applies to the live interactive streaming channel profile only.

 @param metadataDataSource The AgoraMediaMetadataDataSource protocol.
 @param type The metadata type. See [AgoraMetadataType](AgoraMetadataType). Currently, the SDK supports video metadata only.

 @return * YES: Success.
 * NO: Failure.
 */
- (BOOL)setMediaMetadataDataSource:(id<AgoraMediaMetadataDataSource> _Nullable) metadataDataSource withType:(AgoraMetadataType)type;
/** Sets the delegate of the metadata.

 You need to implement the AgoraMediaMetadataDelegate protocol and specify the type of metadata in this method.

 **Note**

 - Call this method before the [joinChannelByToken]([AgoraRtcChannel joinChannelByToken:info:uid:options:]) method.
 - This method applies to the live interactive streaming channel profile only.

 @param metadataDelegate The AgoraMediaMetadataDelegate protocol.
 @param type The metadata type. See [AgoraMetadataType](AgoraMetadataType). Currently, the SDK supports video metadata only.

 @return * YES: Success.
 * NO: Failure.
 */
- (BOOL)setMediaMetadataDelegate:(id<AgoraMediaMetadataDelegate> _Nullable) metadataDelegate withType:(AgoraMetadataType)type;

@end
/** The AgoraRtcChannelDelegate class.

 **Since** v3.0.0.
 */
@protocol AgoraRtcChannelDelegate <NSObject>
@optional
/** Reports a warning during SDK runtime.

 In most cases, the app can ignore the warning reported by the SDK because the SDK can usually fix the issue and resume running.

 For instance, the SDK may report an AgoraWarningCodeOpenChannelTimeout(106) warning upon disconnection from the server and attempts to reconnect.

 See [AgoraWarningCode](AgoraWarningCode).

 @param rtcChannel AgoraRtcChannel
 @param warningCode Warning code: AgoraWarningCode
 */
- (void)rtcChannel:(AgoraRtcChannel * _Nonnull)rtcChannel
               didOccurWarning:(AgoraWarningCode)warningCode;
/** Reports an error during SDK runtime.

In most cases, the SDK cannot fix the issue and resume running. The SDK requires the app to take action or informs the user about the issue.

For example, the SDK reports an AgoraErrorCodeStartCall = 1002 error when failing to initialize a call. The app informs the user that the call initialization failed and invokes the [leaveChannel]([AgoraRtcChannel leaveChannel]) method to leave the channel.

See [AgoraErrorCode](AgoraErrorCode).

 @param rtcChannel AgoraRtcChannel
 @param errorCode Error code: AgoraErrorCode
 */
- (void)rtcChannel:(AgoraRtcChannel * _Nonnull)rtcChannel
                 didOccurError:(AgoraErrorCode)errorCode;
/** Occurs when the local user joins a specified channel.

 @param rtcChannel AgoraRtcChannel
 @param uid User ID. If the `uid` is specified in the [joinChannelByToken]([AgoraRtcChannel joinChannelByToken:info:uid:options:]) method, the specified user ID is returned. If the user ID is not specified when the `joinChannelByToken` method is called, the server automatically assigns a `uid`.
 @param elapsed Time elapsed (ms) from the user calling the `joinChannelByToken` method until the SDK triggers this callback.
 */
- (void)rtcChannelDidJoinChannel:(AgoraRtcChannel * _Nonnull)rtcChannel
                            withUid:(NSUInteger)uid
                            elapsed:(NSInteger) elapsed;
/** Occurs when the local user rejoins a channel.

 If the client loses connection with the server because of network problems, the SDK automatically attempts to reconnect and then triggers this callback upon reconnection, indicating that the user rejoins the channel with the assigned channel ID and user ID.

 @param rtcChannel AgoraRtcChannel
 @param uid     User ID. If the `uid` is specified in the [joinChannelByToken]([AgoraRtcChannel joinChannelByToken:info:uid:options:]) method, the specified user ID is returned. If the user ID is not specified when the `joinChannelByToken` method is called, the server automatically assigns a `uid`.
 @param elapsed Time elapsed (ms) from starting to reconnect to a successful reconnection.
 */
- (void)rtcChannelDidRejoinChannel:(AgoraRtcChannel * _Nonnull)rtcChannel
                              withUid:(NSUInteger)uid
                              elapsed:(NSInteger) elapsed;
/** Occurs when the local user leaves a channel.

 When the app calls the [leaveChannel]([AgoraRtcChannel leaveChannel]) method, this callback notifies the app that a user leaves a channel.

 With this callback, the app retrieves information, such as the call duration and the statistics.

 @param rtcChannel AgoraRtcChannel
 @param stats  Statistics of the call: [AgoraChannelStats](AgoraChannelStats).
 */
- (void)rtcChannelDidLeaveChannel:(AgoraRtcChannel * _Nonnull)rtcChannel
                           withStats:(AgoraChannelStats * _Nonnull)stats;
/** Occurs when the local user role switches in the live interactive streaming.

The SDK triggers this callback when the local user switches the user role by calling the [setClientRole]([AgoraRtcChannel setClientRole:]) method after joining the channel.

 @param rtcChannel AgoraRtcChannel
 @param oldRole Role that the user switches from: AgoraClientRole.
 @param newRole Role that the user switches to: AgoraClientRole.
 */
- (void)rtcChannel:(AgoraRtcChannel * _Nonnull)rtcChannel
          didClientRoleChanged:(AgoraClientRole)oldRole
                       newRole:(AgoraClientRole)newRole;
/** Occurs when a remote user or host joins a channel.

 - Communication profile: This callback notifies the app that another user joins the channel. If other users are already in the channel, the SDK also reports to the app on the existing users.
 - Live interactive streaming profile: This callback notifies the app that a host joins the channel. If other hosts are already in the channel, the SDK also reports to the app on the existing hosts. Agora recommends limiting the number of hosts to 17.

 The SDK triggers this callback under one of the following circumstances:
 - A remote user/host joins the channel by calling the [joinChannelByToken]([AgoraRtcChannel joinChannelByToken:info:uid:options:]) method.
 - A remote user switches the user role to the host by calling the [setClientRole]([AgoraRtcChannel setClientRole:]) method after joining the channel.
 - A remote user/host rejoins the channel after a network interruption.
 - A host injects an online media stream into the channel by calling the [addInjectStreamUrl]([AgoraRtcChannel addInjectStreamUrl:config:]) method.

 **Note**

 Live interactive streaming profile:

 * The host receives this callback when another host joins the channel.
 * The audience in the channel receives this callback when a new host joins the channel.
 * When a web application joins the channel, the SDK triggers this callback as long as the web application publishes streams.

 @param rtcChannel AgoraRtcChannel
 @param uid     ID of the user or host who joins the channel. If the `uid` is specified in the `joinChannelByToken` method, the specified user ID is returned. If the `uid` is not specified in the `joinChannelByToken` method, the Agora server automatically assigns a `uid`.
 @param elapsed Time elapsed (ms) from the local user calling the `joinChannelByToken` method until the SDK triggers this callback.
 */
- (void)rtcChannel:(AgoraRtcChannel * _Nonnull)rtcChannel
                didJoinedOfUid:(NSUInteger)uid
                       elapsed:(NSInteger)elapsed;
/** Occurs when a remote user (Communication)/host (Live Broadcast) leaves a channel.

There are two reasons for users to be offline:

- Leave a channel: When the user/host leaves a channel, the user/host sends a goodbye message. When the message is received, the SDK assumes that the user/host leaves a channel.
- Drop offline: When no data packet of the user or host is received for a certain period of time (20 seconds for the Communication profile, and more for the live interactive streaming profile), the SDK assumes that the user/host drops offline. Unreliable network connections may lead to false detections, so Agora recommends using the [Agora RTM SDK](https://docs.agora.io/en/Real-time-Messaging/product_rtm?platform=All%20Platforms) for more reliable offline detection.

 @param rtcChannel AgoraRtcChannel
 @param uid    ID of the user or host who leaves a channel or goes offline.
 @param reason Reason why the user goes offline, see AgoraUserOfflineReason.
 */
- (void)rtcChannel:(AgoraRtcChannel * _Nonnull)rtcChannel
               didOfflineOfUid:(NSUInteger)uid
                        reason:(AgoraUserOfflineReason)reason;
/** Occurs when the network connection state changes.

The SDK triggers this callback to report on the current network connection state when it changes, and the reason of the change.

@param rtcChannel AgoraRtcChannel
@param state The current network connection state, see AgoraConnectionStateType.
@param reason The reason of the connection state change, see AgoraConnectionChangedReason.
 */
- (void)rtcChannel:(AgoraRtcChannel * _Nonnull)rtcChannel
      connectionChangedToState:(AgoraConnectionStateType)state
                        reason:(AgoraConnectionChangedReason)reason;
/** Occurs when the SDK cannot reconnect to Agora's edge server 10 seconds after its connection to the server is interrupted.

The SDK triggers this callback when it cannot connect to the server 10 seconds after calling the [joinChannelByToken]([AgoraRtcChannel joinChannelByToken:info:uid:options:]) method, regardless of whether it is in the channel or not.

If the SDK fails to rejoin the channel 20 minutes after being disconnected from Agora's edge server, the SDK stops rejoining the channel.

@param rtcChannel AgoraRtcChannel
 */
- (void)rtcChannelDidLost:(AgoraRtcChannel * _Nonnull)rtcChannel;
/** Occurs when the token expires in 30 seconds.

 The user becomes offline if the `token` used in the [joinChannelByToken]([AgoraRtcChannel joinChannelByToken:info:uid:options:]) method expires. The SDK triggers this callback 30 seconds before the `token` expires to remind the app to get a new `token`.
 Upon receiving this callback, generate a new `token` on the server and call the [renewToken]([AgoraRtcChannel renewToken:]) method to pass the new `token` to the SDK.

 @param rtcChannel AgoraRtcChannel
 @param token  The `token` that expires in 30 seconds.
 */
- (void)rtcChannel:(AgoraRtcChannel * _Nonnull)rtcChannel
      tokenPrivilegeWillExpire:(NSString *_Nonnull)token;
/** Occurs when the token expires.

 After a `token` is specified by calling the [joinChannelByToken]([AgoraRtcChannel joinChannelByToken:info:uid:options:]) method, if the SDK losses connection to the Agora server due to network issues, the `token` may expire after a certain period of time and a new `token` may be required to reconnect to the server.

 The SDK triggers this callback to notify the app to generate a new `token`. Call the [renewToken]([AgoraRtcChannel renewToken:]) method to renew the `token`.

 @param rtcChannel AgoraRtcChannel
 */
- (void)rtcChannelRequestToken:(AgoraRtcChannel * _Nonnull)rtcChannel;
/** Reports which user is the loudest speaker over a period of time.

This callback reports the speaker with the highest accumulative volume during a certain period.
If the user enables the audio volume indication by calling the `enableAudioVolumeIndication` method, this callback returns the user ID of the active speaker whose voice is detected by the audio volume detection module of the SDK.

**Note:**

* To receive this callback, you need to call the `enableAudioVolumeIndication` method.
* This callback returns the user ID of the user with the highest voice volume during a period of time, instead of at the moment.
 @param rtcChannel AgoraRtcChannel
 @param speakerUid The user ID of the active speaker. A `speakerUid` of 0 represents the local user.
 */
- (void)rtcChannel:(AgoraRtcChannel * _Nonnull)rtcChannel activeSpeaker:(NSUInteger)speakerUid;
/** Occurs when the video size or rotation of a specific remote user changes.

 @param rtcChannel AgoraRtcChannel
 @param uid      User ID of the remote user or local user (0) whose video size or rotation changes.
 @param size     New video size.
 @param rotation New rotation of the video. The value ranges between 0 and 360.
 */
- (void)rtcChannel:(AgoraRtcChannel * _Nonnull)rtcChannel videoSizeChangedOfUid:(NSUInteger)uid size:(CGSize)size rotation:(NSInteger)rotation;
/** Occurs when the remote video state changes.

 @param rtcChannel AgoraRtcChannel
 @param uid ID of the remote user whose video state changes.
 @param state The state of the remote video. See [AgoraVideoRemoteState](AgoraVideoRemoteState).
 @param reason The reason of the remote video state change. See [AgoraVideoRemoteStateReason](AgoraVideoRemoteStateReason).
 @param elapsed The time elapsed (ms) from the local user calling the [joinChannelByToken]([AgoraRtcChannel joinChannelByToken:info:uid:options:]) method until the SDK triggers this callback.
 */
- (void)rtcChannel:(AgoraRtcChannel * _Nonnull)rtcChannel remoteVideoStateChangedOfUid:(NSUInteger)uid state:(AgoraVideoRemoteState)state reason:(AgoraVideoRemoteStateReason)reason elapsed:(NSInteger)elapsed;;
/** Occurs when the local audio state changes.

 This callback indicates the state change of the local audio stream, including the state of the audio recording and encoding, and allows you to troubleshoot issues when exceptions occur.

 @param rtcChannel AgoraRtcChannel
 @param uid ID of the remote user whose audio state changes.
 @param state  State of the remote audio. See [AgoraAudioRemoteState](AgoraAudioRemoteState).
 @param reason The reason of the remote audio state change. See [AgoraAudioRemoteStateReason](AgoraAudioRemoteStateReason).
 @param elapsed Time elapsed (ms) from the local user calling the [joinChannelByToken]([AgoraRtcChannel joinChannelByToken:info:uid:options:]) method until the SDK triggers this callback.
 */
- (void)rtcChannel:(AgoraRtcChannel * _Nonnull)rtcChannel remoteAudioStateChangedOfUid:(NSUInteger)uid state:(AgoraAudioRemoteState)state reason:(AgoraAudioRemoteStateReason)reason elapsed:(NSInteger)elapsed;
/** Occurs when the published video stream falls back to an audio-only stream due to unreliable network conditions or switches back to the video when the network conditions improve.

 If you call the [setLocalPublishFallbackOption]([AgoraRtcEngineKit setLocalPublishFallbackOption:]) method and set `option` as AgoraStreamFallbackOptionAudioOnly, the SDK triggers this callback when the published stream falls back to audio-only mode due to unreliable uplink conditions, or when the audio stream switches back to the video when the uplink network condition improves.

 **Note**

 Once the published stream falls back to audio only, the remote app receives the [userMuteVideoBlock]([AgoraRtcEngineKit userMuteVideoBlock:]) callback.

 @param rtcChannel AgoraRtcChannel
 @param isFallbackOrRecover Whether the published stream falls back to audio-only or switches back to the video:

 * YES: The published stream falls back to audio-only due to unreliable network conditions.
 * NO: The published stream switches back to the video after the network conditions improve.
 */
- (void)rtcChannel:(AgoraRtcChannel * _Nonnull)rtcChannel didLocalPublishFallbackToAudioOnly:(BOOL)isFallbackOrRecover;
/** Occurs when the remote video stream falls back to an audio-only stream due to unreliable network conditions or switches back to the video after the network conditions improve.

 If you call [setRemoteSubscribeFallbackOption]([AgoraRtcEngineKit setRemoteSubscribeFallbackOption:]) and set `option` as `AgoraStreamFallbackOptionAudioOnly`, the SDK triggers this callback when the remote media stream falls back to audio only due to unreliable network conditions or switches back to the video after the network condition improves.

 **Note:**

 Once the remote media stream is switched to the low stream due to unreliable network conditions, you can monitor the stream switch between a high stream and low stream in the [remoteVideoStats]([AgoraRtcChannelDelegate rtcChannel:remoteVideoStats:]) callback.

 @param rtcChannel AgoraRtcChannel
 @param isFallbackOrRecover Whether the remote media stream falls back to audio-only or switches back to the video:

 * YES: The remote media stream falls back to audio-only due to unreliable network conditions.
 * NO: The remote media stream switches back to the video stream after the network conditions improve.
 @param uid ID of the remote user sending the stream.
 */
- (void)rtcChannel:(AgoraRtcChannel * _Nonnull)rtcChannel didRemoteSubscribeFallbackToAudioOnly:(BOOL)isFallbackOrRecover byUid:(NSUInteger)uid;
/** Reports the statistics of the current call. The SDK triggers this callback once every two seconds after the user joins the channel.

 @param rtcChannel AgoraRtcChannel
 @param stats  Statistics of the AgoraRtcEngineKit: [AgoraChannelStats](AgoraChannelStats).
 */
- (void)rtcChannel:(AgoraRtcChannel * _Nonnull)rtcChannel reportRtcStats:(AgoraChannelStats * _Nonnull)stats;
/** Reports the last mile network quality of each user in the channel once every two seconds.

 Last mile refers to the connection between the local device and Agora's edge server. The SDK triggers this callback once every two seconds to report the last mile network conditions of each user in the channel. If a channel includes multiple users, the SDK triggers this callback as many times.

 @param rtcChannel AgoraRtcChannel
 @param uid       User ID. The network quality of the user with this `uid` is reported. If `uid` is 0, the local network quality is reported.
 @param txQuality Uplink transmission quality of the user in terms of the transmission bitrate, packet loss rate, average RTT (Round-Trip Time), and jitter of the uplink network. `txQuality` is a quality rating helping you understand how well the current uplink network conditions can support the selected AgoraVideoEncoderConfiguration. For example, a 1000-Kbps uplink network may be adequate for video frames with a resolution of 640 * 480 and a frame rate of 15 fps in the live interactive streaming profile, but may be inadequate for resolutions higher than 1280 * 720. See  AgoraNetworkQuality.
 @param rxQuality Downlink network quality rating of the user in terms of packet loss rate, average RTT, and jitter of the downlink network.  See AgoraNetworkQuality.
 */
- (void)rtcChannel:(AgoraRtcChannel * _Nonnull)rtcChannel networkQuality:(NSUInteger)uid txQuality:(AgoraNetworkQuality)txQuality rxQuality:(AgoraNetworkQuality)rxQuality;
/** Reports the statistics of the video stream from each remote user/host.

 The SDK triggers this callback once every two seconds for each remote user/host. If a channel includes multiple remote users, the SDK triggers this callback as many times.

 This callback reports the statistics more closely linked to the real-user experience of the video transmission quality than the statistics that the [videoTransportStatsOfUid]([AgoraRtcEngineDelegate rtcEngine:videoTransportStatsOfUid:delay:lost:rxKBitRate:]) callback reports. This callback reports more about media layer statistics such as the frame loss rate, while the `videoTransportStatsOfUid` callback reports more about the transport layer statistics such as the packet loss rate.

Schemes such as FEC (Forward Error Correction) or retransmission counter the frame loss rate. Hence, users may find the overall video quality acceptable even when the packet loss rate is high.

 @param rtcChannel AgoraRtcChannel
 @param stats  Statistics of the received remote video streams. See [AgoraRtcRemoteVideoStats](AgoraRtcRemoteVideoStats).
 */
- (void)rtcChannel:(AgoraRtcChannel * _Nonnull)rtcChannel remoteVideoStats:(AgoraRtcRemoteVideoStats * _Nonnull)stats;
/** Reports the statistics of the audio stream from each remote user/host.

 The SDK triggers this callback once every two seconds for each remote user/host. If a channel includes multiple remote users, the SDK triggers this callback as many times.

 This callback reports the statistics more closely linked to the real-user experience of the audio transmission quality than the statistics that the [audioTransportStatsOfUid]([AgoraRtcEngineDelegate rtcEngine:audioTransportStatsOfUid:delay:lost:rxKBitRate:]) callback reports. This callback reports more about media layer statistics such as the frame loss rate, while the `audioTransportStatsOfUid` callback reports more about the transport layer statistics such as the packet loss rate.

Schemes such as FEC (Forward Error Correction) or retransmission counter the frame loss rate. Hence, users may find the overall audio quality acceptable even when the packet loss rate is high.

 @param rtcChannel AgoraRtcChannel
 @param stats  Statistics of the received remote audio streams. See AgoraRtcRemoteAudioStats.
 */
- (void)rtcChannel:(AgoraRtcChannel * _Nonnull)rtcChannel remoteAudioStats:(AgoraRtcRemoteAudioStats * _Nonnull)stats;
/** Occurs when the state of the RTMP streaming changes.

The SDK triggers this callback to report the result of the local user calling the [addPublishStreamUrl]([AgoraRtcChannel addPublishStreamUrl:transcodingEnabled:]) or [removePublishStreamUrl]([AgoraRtcChannel removePublishStreamUrl:]) method.

This callback returns the URL and its current streaming state.

This callback indicates the state of the RTMP streaming. When exceptions occur, you can troubleshoot issues by referring to the detailed error descriptions in the `errorCode` parameter.

@param rtcChannel AgoraRtcChannel
@param url The RTMP URL address.
@param state The RTMP streaming state: AgoraRtmpStreamingState. When the streaming state is AgoraRtmpStreamingStateFailure(4), see the `errorCode` parameter for details.
@param errorCode The detailed error information for streaming: AgoraRtmpStreamingErrorCode.
 */
- (void)rtcChannel:(AgoraRtcChannel * _Nonnull)rtcChannel rtmpStreamingChangedToState:(NSString * _Nonnull)url state:(AgoraRtmpStreamingState)state errorCode:(AgoraRtmpStreamingErrorCode)errorCode;

/** Reports events during the RTMP streaming.
 
 @param rtcChannel AgoraRtcChannel
 @param url The RTMP streaming URL.
 @param eventCode The event code. See AgoraRtmpStreamingEvent.
 */
- (void)rtcChannel:(AgoraRtcChannel * _Nonnull)rtcChannel rtmpStreamingEventWithUrl:(NSString * _Nonnull)url eventCode:(AgoraRtmpStreamingEvent)eventCode;

/** Occurs when the CDN live streaming settings are updated.

 When the `LiveTranscoding` class in the [setLiveTranscoding]([AgoraRtcChannel setLiveTranscoding:]) method updates, the SDK triggers the `rtcChannelTranscodingUpdated` callback to report the update information to the local host.

 **Note:**

 If you call the `setLiveTranscoding` method to set the `LiveTranscoding` class for the first time, the SDK does not trigger the `rtcChannelTranscodingUpdated` callback.

 @param rtcChannel AgoraRtcChannel
 */
- (void)rtcChannelTranscodingUpdated:(AgoraRtcChannel * _Nonnull)rtcChannel;


#pragma mark Inject Stream URL Delegate Methods
/** Reports the status of injecting an online stream to the live interactive streaming.

@param rtcChannel AgoraRtcChannel
@param url     URL address of the externally injected stream.
@param uid     User ID.
@param status  Status of the externally injected stream. See AgoraInjectStreamStatus.
 */
- (void)rtcChannel:(AgoraRtcChannel * _Nonnull)rtcChannel streamInjectedStatusOfUrl:(NSString * _Nonnull)url uid:(NSUInteger)uid status:(AgoraInjectStreamStatus)status;

#pragma mark Stream Message Delegate Methods

/** Occurs when the local user receives the data stream from a remote user within five seconds.

The SDK triggers this callback when the local user receives the stream message that the remote user sends by calling the [sendStreamMessage]([AgoraRtcChannel sendStreamMessage:data:]) method.

 @param rtcChannel AgoraRtcChannel
 @param uid      User ID of the remote user sending the message.
 @param streamId Stream ID.
 @param data     Data received by the local user.
 */
- (void)rtcChannel:(AgoraRtcChannel * _Nonnull)rtcChannel receiveStreamMessageFromUid:(NSUInteger)uid streamId:(NSInteger)streamId data:(NSData * _Nonnull)data;
/** Occurs when the local user does not receive the data stream from the remote user within five seconds.

 The SDK triggers this callback when the local user fails to receive the stream message that the remote user sends by calling the [sendStreamMessage]([AgoraRtcChannel sendStreamMessage:data:]) method.

 @param rtcChannel AgoraRtcChannel
 @param uid      User ID of the remote user sending the message.
 @param streamId Stream ID.
 @param error    Error code. See AgoraErrorCode.
 @param missed Number of lost messages.
 @param cached Number of incoming cached messages when the data stream is interrupted.
 */
- (void)rtcChannel:(AgoraRtcChannel * _Nonnull)rtcChannel didOccurStreamMessageErrorFromUid:(NSUInteger)uid streamId:(NSInteger)streamId error:(NSInteger)error missed:(NSInteger)missed cached:(NSInteger)cached;
/** Occurs when the state of the media stream relay changes.

 The SDK reports the state of the current media relay and possible error messages in this callback.

 @param rtcChannel AgoraRtcChannel
 @param state The state code in [AgoraChannelMediaRelayState](AgoraChannelMediaRelayState).
 @param error The error code in [AgoraChannelMediaRelayError](AgoraChannelMediaRelayError).
 */
- (void)rtcChannel:(AgoraRtcChannel * _Nonnull)rtcChannel channelMediaRelayStateDidChange:(AgoraChannelMediaRelayState)state error:(AgoraChannelMediaRelayError)error;
/** Reports events during the media stream relay.

 @param rtcChannel AgoraRtcChannel
 @param event The event code in [AgoraChannelMediaRelayEvent](AgoraChannelMediaRelayEvent).
 */
- (void)rtcChannel:(AgoraRtcChannel * _Nonnull)rtcChannel didReceiveChannelMediaRelayEvent:(AgoraChannelMediaRelayEvent)event;
/** Occurs when the audio publishing state changes.

 @since v3.1.0

 This callback indicates the publishing state change of the local audio stream.

 @param rtcChannel AgoraRtcChannel
 @param oldState   The previous publishing state. For details, see AgoraStreamPublishState.
 @param newState   The current publishing state. For details, see AgoraStreamPublishState.
 @param elapseSinceLastState The time elapsed (ms) from the previous state to the current state.
 */
- (void)rtcChannel:(AgoraRtcChannel * _Nonnull)rtcChannel didAudioPublishStateChange:(AgoraStreamPublishState)oldState newState:(AgoraStreamPublishState)newState elapseSinceLastState:(NSInteger)elapseSinceLastState;


/** Occurs when the video publishing state changes.

 @since v3.1.0

 This callback indicates the publishing state change of the local video stream.

 @param rtcChannel AgoraRtcChannel
 @param oldState   The previous publishing state. For details, see AgoraStreamPublishState.
 @param newState   The current publishing state. For details, see AgoraStreamPublishState.
 @param elapseSinceLastState The time elapsed (ms) from the previous state to the current state.
 */
- (void)rtcChannel:(AgoraRtcChannel * _Nonnull)rtcChannel didVideoPublishStateChange:(AgoraStreamPublishState)oldState newState:(AgoraStreamPublishState)newState elapseSinceLastState:(NSInteger)elapseSinceLastState;


/** Occurs when the audio subscribing state changes.

 @since v3.1.0

 This callback indicates the subscribing state change of a remote audio stream.
 @param rtcChannel AgoraRtcChannel
 @param uid       The ID of the remote user.
 @param oldState  The previous subscribing state. For details, see AgoraStreamSubscribeState.
 @param newState  The current subscribing state. For details, see AgoraStreamSubscribeState.
 @param elapseSinceLastState The time elapsed (ms) from the previous state to the current state.
 */
- (void)rtcChannel:(AgoraRtcChannel * _Nonnull)rtcChannel didAudioSubscribeStateChange:(NSUInteger)uid oldState:(AgoraStreamSubscribeState)oldState newState:(AgoraStreamSubscribeState)newState elapseSinceLastState:(NSInteger)elapseSinceLastState;

/** Occurs when the video subscribing state changes.

 @since v3.1.0
 This callback indicates the subscribing state change of a remote video stream.

 @param rtcChannel AgoraRtcChannel
 @param uid       The ID of the remote user.
 @param oldState  The previous subscribing state. For details, see AgoraStreamSubscribeState.
 @param newState  The current subscribing state. For details, see AgoraStreamSubscribeState.
 @param elapseSinceLastState The time elapsed (ms) from the previous state to the current state.
 */
- (void)rtcChannel:(AgoraRtcChannel * _Nonnull)rtcChannel didVideoSubscribeStateChange:(NSUInteger)uid oldState:(AgoraStreamSubscribeState)oldState newState:(AgoraStreamSubscribeState)newState elapseSinceLastState:(NSInteger)elapseSinceLastState;


@end
