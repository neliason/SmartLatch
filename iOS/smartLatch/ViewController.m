

#import "ViewController.h"

static NSString *deviceCode;
static NSString *deviceKey;

@interface ViewController ()

@end

@implementation ViewController
@synthesize myWebView;
@synthesize segmentedControl;
@synthesize latchMode;
@synthesize speed;
@synthesize revolutions;
@synthesize revsField;
@synthesize codeInput;
@synthesize keyInput;


- (void)viewDidLoad
{
    [super viewDidLoad];
    
    deviceCode = @"gsiot-nwn4-nr1x"; //default my deviceCode
    deviceKey = @"i1v9UTgQoKuNF2UBuy2GOWlxJ8E80BlomUUPFM8l"; //default my deviceKey
    NSURL *url = [NSURL URLWithString:@"http://gsiot-nwn4-nr1x.try.yaler.net/i1v9UTgQoKuNF2UBuy2GOWlxJ8E80BlomUUPFM8l/initialize"];
    NSURLRequest *req = [NSURLRequest requestWithURL:url];
    [myWebView loadRequest:req];
    codeInput.delegate = self; //required for closing keyboard
    keyInput.delegate = self;
	// Do any additional setup after loading the view, typically from a nib.
}

- (void)viewDidUnload
{
    [self setMyWebView:nil];
    [super viewDidUnload];
    // Release any retained subviews of the main view.
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    return (interfaceOrientation != UIInterfaceOrientationPortraitUpsideDown);
}


//clockwise, counterclockwise button handler
- (IBAction)segmentedControl:(UISegmentedControl *)sender {
    UISegmentedControl *theSegment = (UISegmentedControl *) sender;
    if (theSegment.selectedSegmentIndex == 0) {
        NSURL *url = [NSURL URLWithString:[NSString stringWithFormat:@"http://%@.try.yaler.net/%@/open", deviceCode, deviceKey]];
        NSURLRequest *req = [NSURLRequest requestWithURL:url];
        [myWebView loadRequest:req];
    }
    else {
        NSURL *url = [NSURL URLWithString:[NSString stringWithFormat:@"http://%@.try.yaler.net/%@/close", deviceCode, deviceKey]];
        NSURLRequest *req = [NSURLRequest requestWithURL:url];
        [myWebView loadRequest:req];
    }
}

//speed setting handler, speed1 = slow, speed2 = fast
- (IBAction)speed:(UISegmentedControl *)sender {
    UISegmentedControl *speedSegment = (UISegmentedControl *) sender;
    if (speedSegment.selectedSegmentIndex == 0) { //fast
        NSURL *url = [NSURL URLWithString:[NSString stringWithFormat:@"http://%@.try.yaler.net/%@/speed2", deviceCode, deviceKey]];
        NSURLRequest *req = [NSURLRequest requestWithURL:url];
        [myWebView loadRequest:req];
    }
    else { //slow
        NSURL *url = [NSURL URLWithString:[NSString stringWithFormat:@"http://%@.try.yaler.net/%@/speed1", deviceCode, deviceKey]];
        NSURLRequest *req = [NSURLRequest requestWithURL:url];
        [myWebView loadRequest:req];
    }
}

//slider value change handler updates label with value
- (IBAction)sliderValueChanged:(UISlider *)sender {
    int value = (int) sender.value;
    NSString *theString = [NSString stringWithFormat:@"%d",value];
    revsField.text = theString; //updates label next to slider
    
}

//"Set" revs button handler sends number of revolutions to motor component
- (IBAction)revsButtonTouchUp:(UIButton *)sender {
    NSString *revs = revsField.text;
    NSURL *url = [NSURL URLWithString:[NSString stringWithFormat:@"http://%@.try.yaler.net/%@/revs%@", deviceCode, deviceKey, revs]];
    NSURLRequest *req = [NSURLRequest requestWithURL:url];
    [myWebView loadRequest:req];
}

//"Go" button handler actuates motor
- (IBAction)goButtonTouchUp:(UIButton *)sender {
    NSURL *url = [NSURL URLWithString:[NSString stringWithFormat:@"http://%@.try.yaler.net/%@/go", deviceCode, deviceKey]];
    NSURLRequest *req = [NSURLRequest requestWithURL:url];
    [myWebView loadRequest:req];
}

//for closing keyboard
- (IBAction)codeEditingEnd:(UITextField *)sender {
    
    //close keyboard
    [sender resignFirstResponder];
    [codeInput resignFirstResponder];
}

//Sets device code and sends command to initialize device
- (IBAction)setCodeBtnTouchUp:(UIButton *)sender {
    //set device code to direct commands to correct URL of device
    deviceCode = codeInput.text;
    
    //save code for when app reopened
    [[NSUserDefaults standardUserDefaults] setObject:deviceCode forKey:@"code"];
    [[NSUserDefaults standardUserDefaults] synchronize];
    
    //set and send URL with initialize command
    NSURL *url = [NSURL URLWithString:[NSString stringWithFormat:@"http://%@.try.yaler.net/%@/initialize", deviceCode, deviceKey]];
    NSURLRequest *req = [NSURLRequest requestWithURL:url];
    [myWebView loadRequest:req];
}

//for closing keyboard
- (IBAction)keyeditingEnd:(UITextField *)sender {
    [sender resignFirstResponder];
    [keyInput resignFirstResponder];
}

//set key and sends command to initialize device
- (IBAction)keySetBtnTouchUp:(UIButton *)sender {
    deviceKey = keyInput.text;
    
    //store key for when app reopened
    [[NSUserDefaults standardUserDefaults] setObject:deviceKey forKey:@"key"];
    [[NSUserDefaults standardUserDefaults] synchronize];
    
    //set and send URL with initialize command
    NSURL *url = [NSURL URLWithString:[NSString stringWithFormat:@"http://%@.try.yaler.net/%@/initialize", deviceCode, deviceKey]];
    NSURLRequest *req = [NSURLRequest requestWithURL:url];
    [myWebView loadRequest:req];
}

//keyboard close helper function
- (BOOL)textFieldShouldReturn:(UITextField *)textField {
    [textField resignFirstResponder];
    return NO;
}


@end
