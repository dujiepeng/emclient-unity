//
//  EMMessageReaction+Helper.h
//  im_flutter_sdk
//
//  Created by 杜洁鹏 on 2022/5/18.
//

#import <HyphenateChat/HyphenateChat.h>
#import "EaseModeToJson.h"
NS_ASSUME_NONNULL_BEGIN

@interface EMMessageReaction (Helper) <EaseModeToJson>
- (nonnull NSDictionary *)toJson;
@end

NS_ASSUME_NONNULL_END