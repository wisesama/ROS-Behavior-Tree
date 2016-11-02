#include <Conditions/ROSCondition.h>


enum Status {RUNNING,SUCCESS, FAILURE};


BT::ROSCondition::ROSCondition(std::string Name) : ConditionNode::ConditionNode(Name)
{
    Type = BT::Condition;

    // Thread start
    Thread = boost::thread(&ROSCondition::Exec, this);
}

BT::ROSCondition::~ROSCondition() {}

void BT::ROSCondition::Exec()
{

   // ROS_INFO("Waiting For the Acutator %s to start", Name);

    actionlib::SimpleActionClient<behavior_tree_core::BTAction> ac(Name, true);
    ac.waitForServer(); //will wait for infinite time until the server starts
    //ROS_INFO("Actuator %s Started", Name);

    behavior_tree_core::BTGoal goal;
    while(true)
    {
	
        // Waiting for a tick to come
        Semaphore.Wait();
        node_result.status = 0;

        ROS_INFO("I am running the request");


        if(ReadState() == Exit)
        {
            // The behavior tree is going to be destroied
            return;
        }

        // Condition checking and state update
       // do{
            ac.sendGoal(goal);
            ac.waitForResult(ros::Duration(30.0));
            node_result = *(ac.getResult());

            std::cout << " Condition Status" << node_result.status << "!" << std::endl;


       // }while(node_result.status != 2 && node_result.status != 1 ); //if it is not halted and has not returned a status

        if (node_result.status == SUCCESS)
        {
            SetNodeState(BT::Success);
           std::cout << Name << " returning Success" << BT::Success << "!" << std::endl;
        }
        else if( node_result.status == FAILURE)
        {
            SetNodeState(BT::Failure);
            std::cout << Name << " returning Failure" << BT::Failure << "!" << std::endl;
        }else{
            SetNodeState(BT::Failure);
            std::cout << Name << " returning NOTHING" << BT::Failure << "!" << std::endl;
        }

	

        // Resetting the state
        WriteState(BT::Idle);
    }
}
