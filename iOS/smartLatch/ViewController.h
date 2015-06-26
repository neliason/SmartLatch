

#import <UIKit/UIKit.h>

@interface ViewController : UIViewController
@property (weak, nonatomic) IBOutlet UIWebView *myWebView;
@property (weak, nonatomic) IBOutlet UISegmentedControl *segmentedControl;
@property (weak, nonatomic) IBOutlet UISegmentedControl *latchMode;
@property (weak, nonatomic) IBOutlet UISegmentedControl *speed;
@property (weak, nonatomic) IBOutlet UITextField *revolutions;
@property (weak, nonatomic) IBOutlet UILabel *revsField;
@property (strong, nonatomic) IBOutlet UISlider *slider;
- (IBAction)sliderValueChanged:(UISlider *)sender;
@property (strong, nonatomic) IBOutlet UIButton *revsButton;
- (IBAction)revsButtonTouchUp:(UIButton *)sender;
@property (strong, nonatomic) IBOutlet UIButton *goButton;
- (IBAction)goButtonTouchUp:(UIButton *)sender;
@property (strong, nonatomic) IBOutlet UITextField *codeInput;
- (IBAction)codeEditingEnd:(UITextField *)sender;
@property (strong, nonatomic) IBOutlet UIButton *setCodeBtn;
- (IBAction)setCodeBtnTouchUp:(UIButton *)sender;
@property (strong, nonatomic) IBOutlet UITextField *keyInput;
- (IBAction)keyeditingEnd:(UITextField *)sender;
@property (strong, nonatomic) IBOutlet UIButton *seySetBtn;
- (IBAction)keySetBtnTouchUp:(UIButton *)sender;


@end
