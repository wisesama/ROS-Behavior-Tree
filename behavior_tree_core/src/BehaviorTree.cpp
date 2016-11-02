#include<BehaviorTree.h>



void Execute(BT::ControlNode* root,int TickPeriod_milliseconds)
{
    std::cout << "Start Drawing!" << std::endl;
    // Starts in another thread the drawing of the BT
    boost::thread t(&drawTree ,root);

    root->ResetColorState();

    while(true)
    {
        std::cout << "Ticking the root node !" << std::endl << std::endl;

        // Ticking the root node
        root->Semaphore.Signal();
        // Printing its state
        root->GetNodeState();

        if(root->ReadState() != BT::Running  )
        {
            //when the root returns a status it resets the colors of the tree
            root->ResetColorState();
        }
          boost::this_thread::sleep(boost::posix_time::milliseconds(TickPeriod_milliseconds));
    }

}
